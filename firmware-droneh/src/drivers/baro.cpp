#include "drivers/baro.h"

Baro::Baro()
: m_baro()
{}

bool Baro::init()
{
    return m_baro.begin(BMP085_ULTRAHIGHRES, &SYS_I2C);
}

void Baro::update()
{
    m_last_measurement.baro.pressure    = m_baro.readPressure();
    m_last_measurement.baro.temperature = m_baro.readTemperature();
    m_last_measurement.baro.altitude    = m_baro.readAltitude();
    m_last_measurement.timestamp        = millis();
}


Baro baro;
