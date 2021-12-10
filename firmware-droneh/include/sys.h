#ifndef SYS_H
#define SYS_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "error.h"

/* -- Pin definitions -- */

// I2C
#define SYS_I2C_SDA          (6u)
#define SYS_I2C_SCL          (7u)
#define SYS_I2C              Wire1

// SPI
#define SYS_RADIO_MISO       (12u)
#define SYS_RADIO_MOSI       (11u)
#define SYS_RADIO_SCK        (10u)
#define SYS_RADIO_CSN        (13u)
#define SYS_RADIO_CE         (14u)
#define SYS_RADIO_SPI         SPI1

// Serial
#define SYS_SERIAL            SerialUSB
#define SYS_SERIAL_BAUD_RATE  115200

// Leds
#define SYS_LED_RX            (2u)
#define SYS_LED_STATUS        (3u)

// Motors
#define SYS_MOTOR_1            (21u)
#define SYS_MOTOR_2            (20u)
#define SYS_MOTOR_3            (19u)
#define SYS_MOTOR_4            (18u)

// Battery & Power supply
#define SYS_INPUT_VOLTAGE      (29u)
#define SYS_USB_POWER          (24u)

// Estimator
#define USE_ESTIMATOR_COMPLEMENTARY_FILTER
// #define USE_ESTIMATOR_KALMAN_FITER

// Filters
#define MOVING_AVERAGE_FILTER_MAX_ORDER 64
#define USE_LOWPASS_FILTER_FOR_ACCELERATION
#define MOVING_AVERAGE_FILTER_ORDER 10

// Frequencies
#define ESTIMATOR_UPDATE_FREQUENCY 100
#define IMU_UPDATE_FREQUENCY 100


// VL53L0X 0x29
// BMP180  0x77
// MPU6050 0x68

bool driverErrors();

void initSystem();
void heartBeat();
void scan();

#endif /* SYS_H */
