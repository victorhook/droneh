#ifndef SENSOR_H
#define SENSOR_H

#include "stdint.h"
#include "driver.h"


typedef struct {
    float acc_x;
    float acc_y;
    float acc_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float temperature;
} imu_measurement_t;

typedef struct {
    uint16_t altitude;
} tof_measurement_t;

typedef struct {
    float temperature;
    float pressure;
    float altitude;
} baro_measurement_t;

typedef enum {
    MEASUREMENT_TYPE_IMU,
    MEASUREMENT_TYPE_TOF,
    MEASUREMENT_TYPE_BARO,
} sensor_type_e;

typedef struct {
    union {
        imu_measurement_t  imu;
        tof_measurement_t  tof;
        baro_measurement_t baro;
    };
    uint64_t timestamp;
    sensor_type_e type;
} sensor_measurement_t;

typedef struct {
    sensor_measurement_t imu;
    sensor_measurement_t baro;
    sensor_measurement_t tof;
} sensor_readings_t;


class AbstractSensor : public Driver {
    public:
        const sensor_measurement_t& read() const;
        virtual bool init() = 0;
        virtual void update() = 0;
        virtual void calibrate();
    protected:
        sensor_measurement_t m_last_measurement;
        bool                 m_is_calibrated;
};


extern sensor_readings_t sensor_readings;


#endif /* SENSOR_H */
