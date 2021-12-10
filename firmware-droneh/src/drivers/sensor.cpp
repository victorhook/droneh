#include "drivers/sensor.h"


AbstractSensor::AbstractSensor(const uint16_t sampling_frequency)
: m_sampling_frequency(sampling_frequency)
{
    m_sampling_period = ( (float) 1.0f / sampling_frequency) * 1000;
}

void AbstractSensor::calibrate()
{
    doCalibrate();
    m_is_calibrated = true;
}

void AbstractSensor::update()
{
    if (!m_is_calibrated)
        calibrate();

    unsigned long now = millis();
    if ((now - m_ms_since_last_sample) > m_sampling_period) {
        doUpdate();
        m_ms_since_last_sample = now;
    }
}

const sensor_measurement_t& AbstractSensor::read()
{
    return m_last_measurement;
}
