import struct
from dataclasses import dataclass, field
from typing import Protocol
import typing as t

from client import log


class PacketTypes:
    PACKET_TYPE_RAW_MOTOR_THRUST = 0
    PACKET_TYPE_RAW_MOTOR_THRUST_ACK = 1
    PACKET_TYPE_SETPOINT_ATTITUDE = 2
    PACKET_TYPE_LOGGING_DOWNLOAD = 3
    PACKET_TYPE_LOGGING_DOWNLOAD_ACK = 4
    PACKET_TYPE_LOGGING_DOWNLOAD_ACK_DONE = 5
    PACKET_TYPE_LOGGING_PARAM_READ = 6
    PACKET_TYPE_LOGGING_PARAM_READ_ACK = 7
    PACKET_TYPE_ACK = 8
    PACKET_TYPE_CONNECT_REQUEST = 9
    PACKET_TYPE_CONNECT_REQUEST_ACK = 10
    PACKET_TYPE_DISCONNECT = 11
    PACKET_TYPE_DISCONNECT_ACK = 12
    PACKET_TYPE_INCORRECT_PACKET_TYPE = 13
    PACKET_TYPE_PING = 14
    PACKET_TYPE_PONG = 15
    PACKET_TYPE_STATUS_UPDATE = 16
    PACKET_TYPE_STATUS_UPDATE_ACK = 17
    PACKET_TYPE_ARM_SET = 18
    PACKET_TYPE_ARM_SET_ACK = 19

    _STRINGS = {
        0: "PACKET_TYPE_RAW_MOTOR_THRUST",
        1: "PACKET_TYPE_RAW_MOTOR_THRUST_ACK",
        2: "PACKET_TYPE_SETPOINT_ATTITUDE",
        3: "PACKET_TYPE_LOGGING_DOWNLOAD",
        4: "PACKET_TYPE_LOGGING_DOWNLOAD_ACK",
        5: "PACKET_TYPE_LOGGING_DOWNLOAD_ACK_DONE",
        6: "PACKET_TYPE_LOGGING_PARAM_READ",
        7: "PACKET_TYPE_LOGGING_PARAM_READ_ACK",
        8: "PACKET_TYPE_ACK",
        9: "PACKET_TYPE_CONNECT_REQUEST",
        10: "PACKET_TYPE_CONNECT_REQUEST_ACK",
        11: "PACKET_TYPE_DISCONNECT",
        12: "PACKET_TYPE_DISCONNECT_ACK",
        13: "PACKET_TYPE_INCORRECT_PACKET_TYPE",
        14: "PACKET_TYPE_PING",
        15: "PACKET_TYPE_PONG",
        16: "PACKET_TYPE_STATUS_UPDATE",
        17: "PACKET_TYPE_STATUS_UPDATE_ACK",
        18: "PACKET_TYPE_ARM_SET",
        19: "PACKET_TYPE_ARM_SET_ACK",
    }



class Protocols:
    PACKET_PROTOCOL_DRONE = 0
    PACKET_PROTOCOL_CAR = 1
    PACKET_PROTOCOL_AIRPLANE = 2

    _STRINGS = {
        0: 'PACKET_PROTOCOL_DRONE',
        1: 'PACKET_PROTOCOL_CAR',
        2: 'PACKET_PROTOCOL_AIRPLANE',
    }


PACKET_SIZE         = 32
PACKET_PAYLOAD_SIZE = 30
PROTOCOL_IN_USE     = Protocols.PACKET_PROTOCOL_DRONE
PROTOCOL_VERSION    = 1


@dataclass
class Packet:
    protocol: int
    version: int
    pkt_type: int
    payload: bytes
    raw: bytes = None

    def __str__(self) -> str:
        #return f'Protocol: {Protocols._STRINGS[self.protocol]}, ' + \
        #       f'Version: {self.version}, ' + \
        return f'Type: {PacketTypes._STRINGS[self.pkt_type]}, ' + \
               f'Payload: {", ".join(hex(a) for a in self.payload)}'


@dataclass
class Status:
    is_armed: bool


def decode_packet(buf: bytes) -> Packet:
    return Packet(
        protocol = (buf[0] & 0xf0) >> 3,
        version  = buf[0] & 0x0f,
        pkt_type = buf[1],
        payload  = buf[2:],
        raw      = buf
    )

def encode_packet(packet: Packet) -> bytes:
    buf = bytearray(PACKET_SIZE)
    buf[0] = (packet.protocol << 3) | packet.version
    buf[1] = packet.pkt_type
    buf[2:] = packet.payload
    return buf


def decode_log_param(payload: bytes) -> log.LogParam:
    param_id, param_type, param_name_length = struct.unpack('BBB', payload[:3])
    param_type = chr(param_type)
    param_name = str(payload[3:3+param_name_length], encoding='utf-8')

    return log.LogParam(
        id       = param_id,
        log_type = param_type,
        name     = param_name
    )

def make_packet(pkt_type: int, payload: bytes = None) -> Packet:
    if payload is None:
        payload = bytearray(PACKET_PAYLOAD_SIZE)
    packet = Packet(
        protocol=PROTOCOL_IN_USE,
        version=PROTOCOL_VERSION,
        pkt_type=pkt_type,
        payload=payload
    )
    return packet

def make_read_param_packet(params: t.List[log.LogParam]):
    nbr_of_params = len(params)
    param_ids = [param.id for param in params]

    payload = bytearray(PACKET_PAYLOAD_SIZE)
    payload[0] = nbr_of_params
    for i, id in enumerate(param_ids):
        payload[1+i] =  id

    return make_packet(PacketTypes.PACKET_TYPE_LOGGING_PARAM_READ, payload)

def make_arm_set_packet(armed: bool) -> Packet:
    payload = bytearray(PACKET_PAYLOAD_SIZE)
    payload[0] = 1 if armed else 0
    return make_packet(PacketTypes.PACKET_TYPE_ARM_SET, payload)

def make_packet_motor_thrust(m1: int, m2: int, m3: int, m4: int) -> Packet:
    payload = bytearray(PACKET_PAYLOAD_SIZE)
    print(m1, m2, m3, m4)
    struct.pack_into('HHHH', payload, 0, m1, m2, m3, m4)
    return make_packet(PacketTypes.PACKET_TYPE_RAW_MOTOR_THRUST, payload)

def decode_parameters(packet: Packet, params: t.List[log.LogParam]) -> t.List[log.LogParam]:
    # Parameter packet:
    # Values in order that we requested by their ID's.
    i = 0
    for param in params:
        param_size = struct.calcsize(param.log_type)
        param.value = struct.unpack(param.log_type, packet.payload[i:i+param_size])[0]
        i += param_size
        # Not want to break stuff here
        if param.value == float('inf'):
            param.value = 999
    return params

def decode_status(packet: Packet) -> Status:
    return Status(
        packet.payload[0] != 0
    )