#include "imu_fusion.h"
#include "math.h"

/*
float acc_roll;
float acc_pitch;
float gyro_roll;
float gyro_pitch;
float gyro_yaw;
*/

void fuse_imu_measurement(const imu_measurement_t measurement, attitude_t& attitude)
{
    //acc_roll  = atan(measurement.acc_y / sqrt(pow(measurement.acc_x, 2) + pow(measurement.acc_z, 2)));
    //acc_pitch = atan(measurement.acc_x / sqrt(pow(measurement.acc_y, 2) + pow(measurement.acc_z, 2)));
}