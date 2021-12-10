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
    int32_t pressure;
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


class AbstractSensor : public Driver {
    public:
        void update();
        void calibrate();
        const sensor_measurement_t& read();
        virtual bool init() = 0;
    protected:
        AbstractSensor(const uint16_t sampling_frequency);
        virtual void doCalibrate() = 0;
        virtual void doUpdate() = 0;
        sensor_measurement_t m_last_measurement;
        bool                 m_is_calibrated;
        uint16_t             m_sampling_frequency,
                             m_sampling_period;
    private:
        unsigned long        m_ms_since_last_sample;
};



#endif /* SENSOR_H */
