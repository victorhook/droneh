#ifndef ERROR_H
#define ERROR_H

// Drivers
typedef enum {
    DRIVER_BARO,
    DRIVER_IMU,
    DRIVER_TOF,
    DRIVER_RADIO,
    DRIVER_LED_CONTROL,
    DRIVER_MOTOR_CONTROL,
    DRIVER_ACTIVE_CONTROLLER,
    DRIVER_SERIAL_LINK,
    DRIVER_BATTERY
} error_code_drivers_e;

#define PANIC(msg) ;

extern int error;

#endif /* ERROR_H */
