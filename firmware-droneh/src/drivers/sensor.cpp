#include "drivers/sensor.h"


void AbstractSensor::calibrate()
{}

const sensor_measurement_t& AbstractSensor::read() const
{
    return m_last_measurement;
}

sensor_readings_t sensor_readings;
