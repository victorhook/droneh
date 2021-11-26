#ifndef SENSORS_H
#define SENSORS_H

#include "drivers/baro.h"
#include "drivers/mpu.h"
#include "drivers/tof.h"


typedef struct {
    float accX;
    float accY;
    float accZ;
    float gyrX;
    float gyrY;
    float gyrZ;
    float height;
    float pressure;
} sensors_values_t;

class Sensors {
    public:
        void read(sensors_values_t);
};


#endif /* SENSORS_H */
