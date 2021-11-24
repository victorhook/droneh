#ifndef MPU_H
#define MPU_H

#include <Adafruit_MPU6050.h>

#include "driver.h"
#include "sys.h"


class MPU : public Driver
{
    private:
        Adafruit_MPU6050 m_mpu;
    public:
        MPU();
        bool init() override;
        void update() override;
};

extern MPU mpu;

#endif /* MPU_H */
