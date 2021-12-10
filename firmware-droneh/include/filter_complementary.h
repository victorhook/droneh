#ifndef FILTER_COMPLEMENTARY_H
#define FILTER_COMPLEMENTARY_H

#include "filter.h"

#ifdef USE_LOWPASS_FILTER_FOR_ACCELERATION
    #include "filter_lowpass.h"
#endif


class FilterComplementary : public AbstractFilter {
    public:
        FilterComplementary();
        void init() override;
        void filter(const imu_measurement_t raw_measurement, const state_t* current_state, state_t* estimate, const float dt) override;
    private:
        #ifdef USE_LOWPASS_FILTER_FOR_ACCELERATION
            FilterMovingAverage m_moving_average_acc_x,
                                m_moving_average_acc_y,
                                m_moving_average_acc_z;
        #endif
        vec3_t m_acc_filtered,
               m_acc_est,
               m_gyro_est,
               m_dgyro;
        float  m_alpha,
               m_last_gyro_x,
               m_last_gyro_y,
               m_last_gyro_z;
};


#endif /* FILTER_COMPLEMENTARY_H */
