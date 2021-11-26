#ifndef STATE_H
#define STATE_H

#include "stdint.h"

typedef struct {
    uint8_t thrust;
    uint8_t roll;
    uint8_t pitch;
    uint8_t yaw;
} state_t;

typedef struct {
    uint8_t thrust;
    uint8_t roll;
    uint8_t pitch;
    uint8_t yaw;
} setpoint_t;

typedef struct {
    state_t measured;
    state_t estimated;
    state_t target;
} State;

extern State state;

#endif /* STATE_H */
