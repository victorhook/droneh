from os import close
import struct
import serial
from serial.serialutil import SerialException
from abc import ABC, abstractmethod
from dataclasses import dataclass

from client import protocol


@dataclass
class Joystick:
    left_x: int
    left_y: int
    right_x: int
    right_y: int
    left_is_pressed: bool
    right_is_pressed: bool

    fmt: str = 'HHHH??'


class Backend(ABC):

    def __init__(self, timeout_ms: int) -> None:
        self._timeout_ms = timeout_ms

    def set_timeout_ms(self, timeout_ms: int) -> None:
        self._timeout_ms = timeout_ms

    @abstractmethod
    def open(self) -> bool:
        return True

    @abstractmethod
    def close(self) -> bool:
        return True

    @abstractmethod
    def read(self, length: int) -> bytes:
        pass

    @abstractmethod
    def write(self, data: bytes) -> None:
        pass



class DummyBackend(Backend):

    def __init__(self) -> None:
        super().__init__(100)

    def open(self) -> bool:
        pass

    def close(self) -> bool:
        pass

    def read(self, length: int) -> bytes:
        return bytearray(32)

    def write(self, data: bytes) -> None:
        pass


class SerialBackend(Backend):

    def __init__(self, baudrate: int, port: str, timeout_ms: int) -> None:
        super().__init__(timeout_ms / 1000)
        self._log_path = '/home/victor/coding/projects/droneh/gui-droneh/out.log'
        self._log = None
        self._serial = None
        self._baudrate = baudrate
        self._port = port
        self.open()

    def open(self) -> bool:
        try:
            self._serial = serial.Serial(baudrate=self._baudrate, port=self._port, timeout=self._timeout_ms)
            self._serial.flush()
            self._log = open(self._log_path, 'w')
        except SerialException as e:
            print(f'Failed to connect to device on serial port {self._port}')
            self._serial = DummyBackend()
        return True

    def close(self) -> bool:
        self._serial.flush()
        self._serial.close()
        self._log.close()
        self._serial = None
        return True

    def read(self, length: int) -> bytes:
        if self._serial.isOpen():
            data = self._serial.read(length)
            self._log.write(' '.join(hex(a) for a in data))
            self._log.write('\n')
            return data

    def write(self, data: bytes) -> None:
        if self._serial.isOpen():
            self._serial.write(data)
            self._serial.flushOutput()



class SerialProxyBackend(SerialBackend):

    """
        This class acts as a proxy between the PC and the client (droneh).
        It expects the hand-controller to handle the radio communication
        between the drone and the pc.
    """

    PROXY_COMMAND_READ           = 0
    PROXY_COMMAND_WRITE          = 1
    PROXY_COMMAND_READ_JOYSTICKS = 2
    CONNECTED                    = 0x33

    def __init__(self, baudrate: int, port: str, timeout_ms: int) -> None:
        super().__init__(baudrate, port, timeout_ms)

    def open(self) -> bool:
        # Open serial connection
        super().open()

        # Temporarly set connection timeout to 2 seconds (Needed for arduino to reset.)
        timeout = self._timeout_ms
        self._serial.timeout = 2

        # Read 1 byte from arduino on startup -> We're connected.
        if self._serial.read(1) == self.CONNECTED:
            print('Failed to connect to serial backend')
            return self.close()

        self._serial.timeout = timeout

        return True

    def get_joysticks(self) -> Joystick:
        self._write_cmd(self.PROXY_COMMAND_READ_JOYSTICKS)
        data = super().read(struct.calcsize(Joystick.fmt))
        data = struct.unpack_from(Joystick.fmt, data, 0)
        return Joystick(*data)

    def read(self, length: int) -> bytes:
        # Send PROXY READ, then read from serial.
        self._write_cmd(self.PROXY_COMMAND_READ)
        return super().read(length)

    def write(self, data: bytes) -> None:
        # Send PROXY WRITE, then write to serial.
        self._write_cmd(self.PROXY_COMMAND_WRITE)
        return super().write(data)

    def _write_cmd(self, cmd: int) -> None:
        return self._serial.write(struct.pack('B', cmd))


if __name__ == '__main__':
    s = SerialProxyBackend(baudrate=9600, port='/dev/ttyUSB0', timeout_ms=100)
    while 1:
        s.write(b'hey')
        pass