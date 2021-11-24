#ifndef BMP180_H
#define BMP180_H

#include "driver.h"

#define BMP180_READ            0xEF
#define BMP180_WRITE           0xEE
#define BMP180_OSS   (uint8_t) 1


extern TwoWire i2c;


class BMP180 {
    public:
        void init();
        void debug();
        short getTemperature();
        short getPressure();
};


#endif /* BMP180_H */
