#include "estimator.h"
#include "log.h"


Estimator::Estimator(AbstractFilter* filter)
 : m_filter(filter),
   m_last_estimation(0),
   m_update_period((1.0f /ESTIMATOR_UPDATE_FREQUENCY) * 1000)
{
    LOG_ADD_PARAM(F32, "estimate_roll",  &m_estimate.attitude.roll);
    LOG_ADD_PARAM(F32, "estimate_pitch", &m_estimate.attitude.pitch);
    LOG_ADD_PARAM(F32, "estimate_yaw",   &m_estimate.attitude.yaw);
    LOG_ADD_PARAM(U8, "is_armed",   &m_estimate.is_armed);
    LOG_ADD_PARAM(F32, "estimate_roll_deg",  &m_estimate_deg.attitude.roll);
    LOG_ADD_PARAM(F32, "estimate_pitch_deg", &m_estimate_deg.attitude.pitch);
    LOG_ADD_PARAM(F32, "estimate_yaw_deg",   &m_estimate_deg.attitude.yaw);
}

void Estimator::init()
{
    m_filter->init();
}

void Estimator::estimate(const sensor_measurement_t raw_measurement, state_t* current_state)
{
    if (m_last_estimation == 0)
        m_last_estimation = millis();

    // Calculate time difference dt.
    float curr_time = millis();
    float dt_ms = curr_time - m_last_estimation;

    if (dt_ms < m_update_period)
        return;

    float dt = dt_ms / 1000;
    m_last_estimation = curr_time;

    // Update the filter.
    m_filter->filter(raw_measurement.imu, current_state, &m_estimate, dt);

    // Update state with the estimated values.
    current_state->attitude.roll  = m_estimate.attitude.roll;
    current_state->attitude.pitch = m_estimate.attitude.pitch;
    current_state->attitude.yaw   = m_estimate.attitude.yaw;

    // Update state in degrees as well.
    m_estimate_deg.attitude.roll  = m_estimate.attitude.roll  * (180 / PI);
    m_estimate_deg.attitude.pitch = m_estimate.attitude.pitch * (180 / PI);
    m_estimate_deg.attitude.yaw   = m_estimate.attitude.yaw   * (180 / PI);
    //Serial.printf("Roll: %d, Pitch: %d, Yaw: %d\n",  (int) (state_estimated.attitude.roll*(180 / PI)), (int) (state_estimated.attitude.pitch*(180 / PI)), (int) (state_estimated.attitude.yaw*(180 / PI)));

    // Update ARM and modes
    state_estimated.is_armed     = state_target.is_armed;
    state_estimated.is_connected = state_target.is_connected;
    state_estimated.mode         = state_target.mode;
}


#ifdef USE_ESTIMATOR_COMPLEMENTARY_FILTER
    #include "filter_complementary.h"
    AbstractFilter* filter = (AbstractFilter*) new FilterComplementary();
#elif USE_ESTIMATOR_KALMAN_FILTER
    #include "filter_kalman.h"
    AbstractFilter* filter = (AbstractFilter*) new FilterKalman();
#endif

Estimator estimator(filter);
