#ifndef STATE_H
#define STATE_H

#include "types.h"
#include "drivers/motors.h"

typedef struct {
    vec3_t         attitude;
    motor_thrust_t motor_raw;
    uint16_t       thrust;
    bool           is_armed;
} state_t;

typedef state_t setpoint_t;

extern state_t state_estimated;
extern state_t state_target;

#endif /* STATE_H */
