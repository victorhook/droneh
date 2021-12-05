#ifndef IMU_FUSION_H
#define IMU_FUSION_H

#include "drivers/imu.h"


typedef struct {
    float roll;
    float pitch;
    float yaw;
} attitude_t;

void fuse_imu_measurement(const imu_measurement_t measurement, attitude_t& attitude);


#endif /* IMU_FUSION_H */
