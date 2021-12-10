#ifndef IMU_H
#define IMU_H

#include <Adafruit_MPU6050.h>

#include "sensor.h"
#include "sys.h"

#define GRAVITY 9.82f
#define CALIBRATION_READINGS 200


class IMU : public AbstractSensor
{
    public:
        IMU();
        bool init() override;
        void doUpdate() override;
        void doCalibrate() override;

    private:
        void readSensor();
        Adafruit_MPU6050  m_imu;
        imu_measurement_t m_bias;
};

extern IMU imu;
extern sensor_measurement_t measurement_imu;

#endif /* IMU_H */
