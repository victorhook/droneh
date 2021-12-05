#ifndef FILTER_H
#define FILTER_H

#include "drivers/imu.h"
#include "state.h"


class AbstractFilter {
    public:
        virtual void init() = 0;
        virtual void filter(const imu_measurement_t raw_measurement, const state_t* current_state, state_t* estimate, const float dt) = 0;
};


#endif /* FILTER_H */
