#include "droneh_ctrl.h"
#include "state.h"
#include "drivers/leds.h"
#include "drivers/input.h"
#include "drivers/display.h"
#include "drivers/radio.h"


State state;
LedHandler ledHandler(&state);
InputHandler inputHandler(&state);
//DisplayHandler displayHandler(&state);
Radio radio(&state);

#undef DEBUG
#ifdef DEBUG
    #include "drivers/debugger.h"
    Debugger debugger(state, 2000);
#endif


void initDrivers() {
    Serial.begin(9600);
    ledHandler.init();
    inputHandler.init();
    //displayHandler.init();
    radio.init();
    #ifdef DEBUG
        debugger.init();
    #endif
}

void setup() {
    initDrivers();
}

void printValues() {
    Serial.print("LX: "); Serial.print(state.joystick_left.x);
    Serial.print("  LY: "); Serial.print(state.joystick_left.y);
    Serial.print("  LSW: "); Serial.print(state.joystick_left.is_pressed);
    Serial.print("  -  RX: "); Serial.print(state.joystick_right.x);
    Serial.print("  RY: "); Serial.print(state.joystick_right.x);
    Serial.print("  RSW: "); Serial.print(state.joystick_right.is_pressed);
    //Serial.print("  ARM: "); Serial.print(digitalRead(ARM_SWITCH));
    Serial.println("");
}

void loop() {
    while (1) {
        ledHandler.update();
        inputHandler.update();
        //displayHandler.update();
        radio.update();
        #ifdef DEBUG
            debugger.update();
        #endif

        //printValues();
        delay(100);
    }
    // Should never reach this point.
}