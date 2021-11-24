#include "droneh_ctrl.h"
#include "state.h"
#include "drivers/leds.h"
#include "drivers/input.h"
#include "drivers/display.h"
#include "drivers/radio.h"


State state;
LedHandler ledHandler(state);
InputHandler inputHandler(state);
DisplayHandler displayHandler(state);
Radio radio(state);
#ifdef DEBUG
    #include "drivers/debugger.h"
    Debugger debugger(state, 2000);
#endif


void initDrivers() {
    ledHandler.init();
    inputHandler.init();
    displayHandler.init();
    radio.init();
    #ifdef DEBUG
        debugger.init();
    #endif
}

void setup() {
    initDrivers();
}

void loop() {
    while (1) {
        ledHandler.update();
        inputHandler.update();
        displayHandler.update();
        radio.update();
        #ifdef DEBUG
            debugger.update();
        #endif
    }
    // Should never reach this point.
}