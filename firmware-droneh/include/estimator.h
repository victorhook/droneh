#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "filter.h"
#include "state.h"
#include "drivers/sensor.h"

class Estimator {
    public:
        Estimator(AbstractFilter* filter);
        void estimate(const sensor_measurement_t raw_measurement, state_t* current_state);
        void init();
    private:
        AbstractFilter* m_filter;
        state_t         m_estimate,
                        m_estimate_deg;
        float           m_last_estimation;
        size_t          m_update_period;
};

extern Estimator estimator;

#endif /* ESTIMATOR_H */
