#include "state.h"


state_t state_estimated {
    .attitude {
        .roll   = 0,
        .pitch  = 0,
        .yaw    = 0
    },
    .motor_raw {
        .m1 = 0,
        .m2 = 0,
        .m3 = 0,
        .m4 = 0
    },
    .thrust = 0,
    .mode = OPERATING_MODE_NORMAL_CONTROL,
    .is_armed = false,
    .is_connected = false
};

state_t state_target {
    .attitude {
        .roll   = 0,
        .pitch  = 0,
        .yaw    = 0
    },
    .motor_raw {
        .m1 = 0,
        .m2 = 0,
        .m3 = 0,
        .m4 = 0
    },
    .thrust = 0,
    .mode = OPERATING_MODE_NORMAL_CONTROL,
    .is_armed = false,
    .is_connected = false
};
