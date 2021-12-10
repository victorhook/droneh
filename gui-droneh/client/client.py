import struct
import threading
import time
from tkinter import Pack
import typing as t
from typing import Protocol
from queue import Queue
from client import protocol

from client.backends import Backend
from client.callback import Callback
from client import log
from client.log import LogParam
from client.protocol import *
from collections import namedtuple

Task = namedtuple('Task', ['call', 'args', 'kwargs'])

DISCONNECT_TIMEOUT_MS = 1000

def now() -> int:
    return time.time() * 1000


class Client:

    def thread_safe(func):
        """ Decorator that wraps a method to ensure it's being executed
            in the running thread of the client.
            If the caller is NOT in the same thread, the requested call is
            put in a queue, which is being checked regularly in the
            running loop.
        """
        def wrapper(self, *args, **kwargs):
            thread_name = threading.current_thread().getName()
            if thread_name != self._running_thread_name:
                # Caller is from different thread, put task in queue.
                self._queue.put(Task(func, (self, ) + args, kwargs))
                return
            else:
                # Caller is from same thread, it's safe to call it.
                return func(self, *args, **kwargs)


        return wrapper

    def __init__(self, backend: Backend) -> None:
        self.backend = backend

        self.cb_on_connect    = Callback()
        self.cb_on_disconnect = Callback()
        self.cb_on_error      = Callback()
        self.cb_on_arm_change = Callback()

        self._log_configs: t.Dict[str, log.ActiveLogConfig] = {}

        self._trying_to_connect   = False
        self._is_armed            = False
        self._is_logging          = True
        self._is_connected        = False
        self._is_running_flag     = threading.Event()
        self._logs                = []
        self._last_ack            = None
        self._last_status         = 0
        self._running_thread_name = None
        self._queue               = Queue()

    def is_armed(self) -> bool:
        return self._is_armed

    def connect(self):
        if not self.backend.open():
            print('Failed to open backend')
            return

        self._trying_to_connect = True
        packet = self._transmit(PacketType.CONNECT_REQUEST,
                                expects=PacketType.CONNECT_REQUEST_ACK)
        if packet is None:
            return False

        self._trying_to_connect = False

        self._last_ack = now()
        self.cb_on_connect.call('Connected!')
        self._is_connected = True
        self._is_logging = True
        self.cb_on_error.call('Failed to connect to client')

    def start(self):
        # Running thread name is used to know differentiate between functions
        # called from different threads. If this happens, those calls are
        # put in a seperate queue, which executes regularly.
        self._running_thread_name = threading.currentThread().getName()
        with self._queue.mutex:
            self._queue.queue.clear()
        self._is_running_flag.set()
        self._run()

    @thread_safe
    def arm(self):
        packet = protocol.make_arm_set_packet(armed=True)
        if self._transmit_packet(packet, expects=PacketType.ARM_SET_ACK):
            # True means it's armed, False disarmed.
            self.cb_on_arm_change.call(True)

    @thread_safe
    def disarm(self):
        packet = protocol.make_arm_set_packet(armed=False)
        if self._transmit_packet(packet, expects=PacketType.ARM_SET_ACK):
            self.cb_on_arm_change.call(False)

    @thread_safe
    def set_motor_thrust(self, m1: int, m2: int, m3: int, m4: int) -> None:
        if not self._is_connected:
            return
        packet = protocol.make_packet_motor_thrust(m1, m2, m3, m4)
        self._transmit_packet(packet, expects=PacketType.RAW_MOTOR_THRUST_ACK)

    def add_log_config(self, log_config: log.ActiveLogConfig) -> None:
        self._log_configs[log_config.name] = log_config

    def delete_log_config(self, log_config_name: str) -> None:
        self._log_configs.pop(log_config_name)

    @thread_safe
    def disconnect(self):
        self._disconnect()

    def is_connected(self) -> bool:
        return self._is_connected

    def _get_log_params(self):
        # Start getting logging parameters.
        self._transmit(PacketType.LOGGING_DOWNLOAD)

        timeout = 2
        pkt_type = None
        t0 = time.time()

        log_params = []

        while ( (pkt_type != PacketType.LOGGING_DOWNLOAD_ACK_DONE) \
               and ((time.time() - t0) < timeout) ):

            packet = self._receive()
            pkt_type = packet.pkt_type
            if pkt_type == PacketType.LOGGING_DOWNLOAD_ACK:
                log_param = decode_log_param(packet.payload)
                log_params.append(log_param)

        log.save_available_log_params(log_params)

    def _run(self):
        self._get_log_params()
        t0 = now()

        while self._is_running_flag.is_set():
            while not self._queue.empty():
                task = self._queue.get()
                task.call(*task.args, **task.kwargs)

            # Check for timeout.
            self._check_connection()

            # Perform logging.
            self._log()

            # Status check
            self._status_check()

        self._disconnect()

    def _status_check(self):
        t1 = now()

        if t1 - self._last_status:
            packet = self._transmit(PacketType.STATUS_UPDATE,
                             expects=PacketType.STATUS_UPDATE_ACK)
            if not packet:
                return

            status = protocol.decode_status(packet)
            self._is_armed = status.is_armed
            self._last_status = t1

    def _ping(self):
        tnow = now()
        if (tnow - self._last_ack) > 300:
            self._transmit(PacketType.PING)
            pong = self._receive(expects=PacketType.PONG)

    def _log(self):
        if self._is_logging:
            self._request_log_params()

    def _check_connection(self):
        curr_time = now()
        if  (curr_time - self._last_ack) > DISCONNECT_TIMEOUT_MS:
            self._disconnect()

    def _request_log_params(self):
        curr_time = now()
        try:
            for config in self._log_configs.values():
                dt = curr_time - config.last_sent
                if dt > config.frequency_ms:
                    config.last_sent = curr_time
                    params = self._read_params(config.params)
                    config.on_new_data(params)
        except RuntimeError as e:
            print('Runerr %s' % e)

    def _disconnect(self):
        if not self._is_connected:
            return

        self._transmit(PacketType.DISCONNECT)

        packet = self._receive()

        if packet.pkt_type == PacketType.DISCONNECT_ACK:
            print('Disconnect ok')
        else:
            print('Failed to disconnect!')

        self._is_connected = False
        self._is_logging = False
        self._is_running_flag.clear()
        self.backend.close()
        self.cb_on_disconnect.call('Disconnected')

    def _read_params(self, params: t.List[LogParam]) -> t.Dict[str, LogParam]:
        packet = make_read_param_packet(params)
        self._transmit_packet(packet)
        packet = self._receive()

        if packet.pkt_type != PacketType.LOGGING_PARAM_READ_ACK:
            # TODO: This
            print('Got wrong packet type response when reading parameters!')
            print(f'Got: {packet}')
            return

        params = decode_parameters(packet, params)
        params = {param.name: param for param in params}
        return params

    def _transmit(self, pkt_type: int, payload: bytes = None, expects: PacketType = None):
        packet = make_packet(pkt_type, payload)
        return self._transmit_packet(packet, expects)

    def _receive(self, expects: PacketType = None) -> Packet:
        if not self._is_connected and not self._trying_to_connect:
            return

        data = self.backend.read(PACKET_SIZE)
        self._last_ack = now()
        packet = decode_packet(data)

        if expects:
            if packet.pkt_type != expects:
                print(f'Got incorrect response type, expected '
                      f'{expects}, got {packet.pkt_type}')
                return False
        return packet

    def _transmit_packet(self, packet: Packet, expects: PacketType = None) -> Packet:
        if not self._is_connected and not self._trying_to_connect:
            return

        data = encode_packet(packet)
        self.backend.write(data)
        if expects:
            return self._receive(expects)



if __name__ == '__main__':
    from backends import SerialBackend
    client = Client(SerialBackend(9600, '/dev/ttyACM0', 1000))
    client.cb_on_error.add_callback(lambda msg: print(msg))
    client.cb_on_disconnect.add_callback(lambda msg: print(msg))
    client.cb_on_connect.add_callback(lambda msg: print('Connected!'))
    client.backend._serial.flush()
    client.connect()