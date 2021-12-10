#ifndef SYS_H
#define SYS_H

#include <Arduino.h>

// Joysticks
#define JOYSTICK_LEFT_X   A3
#define JOYSTICK_LEFT_Y   A2
#define JOYSTICK_LEFT_SW  8
#define JOYSTICK_RIGHT_X  A1
#define JOYSTICK_RIGHT_Y  A0
#define JOYSTICK_RIGHT_SW 7

// Radio
#define RADIO_CE 9
#define RADIO_CSN 10

// Switches and leds
#define ARM_SWITCH 2
#define LED_BLUE 6
#define LED_PINK 5
#define LED_GREEN 3

// Uart
#define SYS_SERIAL Serial
#define SYS_SERIAL_BAUD_RATE 115200


#endif /* SYS_H */
