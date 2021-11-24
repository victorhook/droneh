#ifndef STATE_H
#define STATE_H

#include "stdint.h"

typedef struct {
    uint8_t thrust;
    uint8_t roll;
    uint8_t pitch;
    uint8_t yaw;
} state_t;

extern state_t current_state;

#endif /* STATE_H */
