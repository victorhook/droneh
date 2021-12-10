#ifndef BARO_H
#define BARO_H

#include "Adafruit_BMP085.h"

#include "drivers/sensor.h"


class Baro : public AbstractSensor
{
    public:
        Baro();
        bool init() override;
        void doUpdate() override;
        void doCalibrate() override;
    private:
        void readSensor();
        float readAltitude();
        uint32_t        m_pressure_at_startup;
        float           m_bias_altitude;
        Adafruit_BMP085 m_baro;
};

extern Baro baro;

#endif /* BARO_H */
