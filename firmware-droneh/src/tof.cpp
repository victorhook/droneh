#include "drivers/tof.h"


ToF::ToF()
    : m_tof()
{

}

bool ToF::init()
{
    m_tof.setBus(&SYS_I2C);
    m_tof.setTimeout(500);
    return m_tof.init();
    /*
    sensor.setDistanceMode(VL53L1X::Long);
    sensor.setMeasurementTimingBudget(50000);
    sensor.startContinuous(50);
    */
}

void ToF::update()
{

}

ToF tof;
