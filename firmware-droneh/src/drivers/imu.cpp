#include "drivers/imu.h"
#include "log.h"


IMU::IMU()
: AbstractSensor(100),
  m_imu()
{}

bool IMU::init()
{
    LOG_ADD_PARAM(F32, "acc_x",  &m_last_measurement.imu.acc_x);
    LOG_ADD_PARAM(F32, "acc_y",  &m_last_measurement.imu.acc_y);
    LOG_ADD_PARAM(F32, "acc_z",  &m_last_measurement.imu.acc_z);
    LOG_ADD_PARAM(F32, "gyro_x", &m_last_measurement.imu.gyro_x);
    LOG_ADD_PARAM(F32, "gyro_y", &m_last_measurement.imu.gyro_y);
    LOG_ADD_PARAM(F32, "gyro_z", &m_last_measurement.imu.gyro_z);
    LOG_ADD_PARAM(F32, "bias_acc_x",  &m_bias.acc_x);
    LOG_ADD_PARAM(F32, "bias_acc_y",  &m_bias.acc_y);
    LOG_ADD_PARAM(F32, "bias_acc_z",  &m_bias.acc_z);
    LOG_ADD_PARAM(F32, "bias_gyro_x", &m_bias.gyro_x);
    LOG_ADD_PARAM(F32, "bias_gyro_y", &m_bias.gyro_y);
    LOG_ADD_PARAM(F32, "bias_gyro_z", &m_bias.gyro_z);
    return m_imu.begin(MPU6050_I2CADDR_DEFAULT, &SYS_I2C, 0);
}

void IMU::doCalibrate()
{
    imu_measurement_t calibration = {.acc_x=0, .acc_y=0, .acc_z=0, .gyro_x=0, .gyro_y=0, .gyro_z=0};

    for (int i = 0; i < CALIBRATION_READINGS; i++) {
        readSensor();
        calibration.acc_x  += m_last_measurement.imu.acc_x;
        calibration.acc_y  += m_last_measurement.imu.acc_y;
        calibration.acc_z  += m_last_measurement.imu.acc_z;
        calibration.gyro_x += m_last_measurement.imu.gyro_x;
        calibration.gyro_y += m_last_measurement.imu.gyro_y;
        calibration.gyro_z += m_last_measurement.imu.gyro_z;
    }

    m_bias.acc_x  = calibration.acc_x  / CALIBRATION_READINGS;
    m_bias.acc_y  = calibration.acc_y  / CALIBRATION_READINGS;
    m_bias.acc_z  = (calibration.acc_z / CALIBRATION_READINGS) - GRAVITY;
    m_bias.gyro_x = calibration.gyro_x / CALIBRATION_READINGS;
    m_bias.gyro_y = calibration.gyro_y / CALIBRATION_READINGS;
    m_bias.gyro_z = calibration.gyro_z / CALIBRATION_READINGS;
}

void IMU::readSensor()
{
    sensors_event_t acc, gyro, temp;
    m_imu.getEvent(&acc, &gyro, &temp);
    m_last_measurement.imu.acc_x       = -acc.acceleration.y;
    m_last_measurement.imu.acc_y       = -acc.acceleration.x;
    m_last_measurement.imu.acc_z       = acc.acceleration.z;
    m_last_measurement.imu.gyro_x      = -gyro.gyro.y;
    m_last_measurement.imu.gyro_y      = -gyro.gyro.x;
    m_last_measurement.imu.gyro_z      = gyro.gyro.z;
    m_last_measurement.imu.temperature = temp.temperature;
    m_last_measurement.timestamp       = millis();
}

void IMU::doUpdate()
{
    readSensor();

    //Serial.printf("Bias: ax: %f ay: %f az: %f,  gx: %f gy: %f gz: %f\n",
    //                m_bias.acc_x, m_bias.acc_y, m_bias.acc_z, m_bias.gyro_x, m_bias.gyro_y, m_bias.gyro_z);

    // Remove bias from accelerometer and gyroscope.
    m_last_measurement.imu.acc_x  -= m_bias.acc_x;
    m_last_measurement.imu.acc_y  -= m_bias.acc_y;
    m_last_measurement.imu.acc_z  -= m_bias.acc_z;
    m_last_measurement.imu.gyro_x -= m_bias.gyro_x;
    m_last_measurement.imu.gyro_y -= m_bias.gyro_y;
    m_last_measurement.imu.gyro_z -= m_bias.gyro_z;

    //Serial.printf("Bias: ax: %f ay: %f az: %f,  gx: %f gy: %f gz: %f\n",
    //                m_last_measurement.imu.acc_x, m_last_measurement.imu.acc_y, m_last_measurement.imu.acc_z, m_last_measurement.imu.gyro_x, m_last_measurement.imu.gyro_y, m_last_measurement.imu.gyro_z);
}


IMU imu;
