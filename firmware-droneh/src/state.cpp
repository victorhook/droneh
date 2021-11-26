#include "state.h"


State state {
    .measured = {
        .thrust = 0,
        .roll   = 0,
        .pitch  = 0,
        .yaw    = 0
    },
    .estimated = {
        .thrust = 0,
        .roll   = 0,
        .pitch  = 0,
        .yaw    = 0
    },
    .target = {
        .thrust = 0,
        .roll   = 0,
        .pitch  = 0,
        .yaw    = 0
    }
};
