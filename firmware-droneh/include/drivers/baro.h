#ifndef BARO_H
#define BARO_H

#include "Adafruit_BMP085.h"

#include "drivers/sensor.h"


class Baro : public AbstractSensor
{
    public:
        Baro();
        bool init() override;
        void update() override;
    private:
        Adafruit_BMP085 m_baro;
};

extern Baro baro;

#endif /* BARO_H */
