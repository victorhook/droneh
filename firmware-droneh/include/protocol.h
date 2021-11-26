#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "stdint.h"

#define PACKET_SIZE         32
#define PACKET_PAYLOAD_SIZE 30


typedef enum {
    PACKET_TYPE_RAW_MOTOR_THRUST,
    PACKET_TYPE_SETPOINT_ATTITUDE,
    PACKET_TYPE_LOGGING_DOWNLOAD,
    PACKET_TYPE_LOGGING_DOWNLOAD_ACK,
    PACKET_TYPE_ACK,
} packet_type_e;

typedef enum {
    PACKET_PROTOCOL_DRONE,
    PACKET_PROTOCOL_CAR,
    PACKET_PROTOCOL_AIRPLANE,
} packet_protocol_e;

typedef struct {
    packet_protocol_e protocol;
    uint8_t           version;
    packet_type_e     type;
    uint8_t           payload[PACKET_PAYLOAD_SIZE];
} packet_t;


#define PROTOCOL_IN_USE   PACKET_PROTOCOL_DRONE
#define PROTOCOL_VERSION  1


void decode_packet(const uint8_t buf[PACKET_SIZE], packet_t& packet);
void encode_packet(uint8_t buf[PACKET_SIZE], const packet_t& packet);


#endif /* PROTOCOL_H */
