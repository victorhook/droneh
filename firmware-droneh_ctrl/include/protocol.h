#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "stdint.h"


#define PROTOCOL_DRONE (uint8_t) 1
#define PROTOCOL_CAR   (uint8_t) 2


typedef struct {
    uint8_t protocol;
    uint8_t type;
    uint8_t* payload;
} packet_t;


class Protocol {

};


#endif /* PROTOCOL_H */
