#include "drivers/baro.h"
#include "filter_lowpass.h"
#define CALIBRATION_READINGS 30

Baro::Baro()
: AbstractSensor(25),
  m_baro(),
  m_bias_altitude(0)
{}

FilterMovingAverage fir(5);

bool Baro::init()
{
    return m_baro.begin(BMP085_ULTRAHIGHRES, &SYS_I2C);
}

void Baro::doUpdate()
{
    readSensor();
    m_last_measurement.baro.altitude -= m_bias_altitude;
    m_last_measurement.baro.altitude = fir.update(m_last_measurement.baro.altitude);
    Serial.printf("Altitude: %f, %f, %f, %d\n", m_last_measurement.baro.altitude, readAltitude(), m_bias_altitude, m_pressure_at_startup);
    //Serial.printf("%d, %f, %f, %f\n", m_last_measurement.baro.pressure, m_last_measurement.baro.temperature, m_last_measurement.baro.altitude, m_bias_altitude);
}

void Baro::readSensor()
{
    m_last_measurement.baro.pressure    = m_baro.readPressure();
    m_last_measurement.baro.temperature = m_baro.readTemperature();
    m_last_measurement.baro.altitude    = readAltitude();
    m_last_measurement.timestamp        = millis();
}

float Baro::readAltitude()
{
    return m_baro.readAltitude(m_baro.readPressure()) * 100;
    return m_baro.readAltitude(m_pressure_at_startup) * 100;
}

void Baro::doCalibrate()
{
    // Find average pressure value at rest.
    m_pressure_at_startup = 0;
    for (int i = 0; i < CALIBRATION_READINGS; i++) {
        m_pressure_at_startup += m_baro.readPressure();
    }
    m_pressure_at_startup /= CALIBRATION_READINGS;

    // Find average altitude at rest, this will be our bias.
    m_bias_altitude = 0;
    for (int i = 0; i < CALIBRATION_READINGS; i++) {
        m_bias_altitude += readAltitude();
    }
    m_bias_altitude /= CALIBRATION_READINGS;
}


Baro baro;
