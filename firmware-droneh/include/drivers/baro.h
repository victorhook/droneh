#ifndef BARO_H
#define BARO_H

#include "Adafruit_BMP085.h"

#include "driver.h"
#include "sys.h"


class Baro : public Driver
{
    private:
        Adafruit_BMP085 m_baro;
    public:
        Baro();
        bool init() override;
        void update() override;
};

extern Baro baro;

#endif /* BARO_H */
