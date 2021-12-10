#include "drivers/tof.h"


ToF::ToF()
: AbstractSensor(50),
  m_tof()
{}

bool ToF::init()
{
    m_tof.setBus(&SYS_I2C);
    m_tof.setTimeout(500);
    if (!m_tof.init())
        return false;
    m_tof.setDistanceMode(VL53L1X::Long);
    m_tof.setMeasurementTimingBudget(50000);
    m_tof.startContinuous(50);
    return true;
}

void ToF::doUpdate()
{
    m_last_measurement.tof.altitude  = m_tof.read();
    m_last_measurement.timestamp     = millis();
}

void ToF::doCalibrate()
{}

ToF tof;
