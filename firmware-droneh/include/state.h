#ifndef STATE_H
#define STATE_H

#include "types.h"
#include "drivers/motors.h"


typedef enum {
    OPERATING_MODE_MOTOR_THRUST_RAW,
    OPERATING_MODE_NORMAL_CONTROL
} operating_mode_e;

typedef struct {
    vec3_t           attitude;
    motor_thrust_t   motor_raw;
    uint16_t         thrust;
    operating_mode_e mode;
    bool             is_armed;
    bool             is_connected;
} state_t;

extern state_t state_estimated;
extern state_t state_target;

#endif /* STATE_H */
