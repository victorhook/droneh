#include "protocol.h"

#include "string.h"


void decode_packet(const uint8_t buf[PACKET_SIZE], packet_t& packet)
{
    packet.protocol = (packet_protocol_e) ( buf[0] & 0x0f );
    packet.version  = ( (buf[0] & 0xf0) >> 3 );
    packet.type     = (packet_type_e) buf[1];
    memcpy(packet.payload, buf, PACKET_PAYLOAD_SIZE);
}

void encode_packet(uint8_t buf[PACKET_SIZE], const packet_t& packet)
{
    buf[0] = ( packet.protocol | (packet.version << 3) );
    buf[1] = packet.type;
    memcpy(&buf[1], packet.payload, PACKET_PAYLOAD_SIZE);
}
