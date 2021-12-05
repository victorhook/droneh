#ifndef FILTER_KALMAN_H
#define FILTER_KALMAN_H

#include "filter.h"


class FilterKalman : public AbstractFilter {
    public:
        FilterKalman();
        void init() override;
        void filter(const imu_measurement_t raw_measurement, const state_t& current_state, state_t* estimate, const float dt) override;
};

#endif /* FILTER_KALMAN_H */
