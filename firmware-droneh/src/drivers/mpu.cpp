#include "drivers/mpu.h"

MPU::MPU()
    : m_mpu()
{

}

bool MPU::init()
{
    return m_mpu.begin(MPU6050_I2CADDR_DEFAULT, &SYS_I2C, 0);
}

void MPU::update()
{

}


MPU mpu;
