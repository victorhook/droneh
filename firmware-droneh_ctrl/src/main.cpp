#include "drivers/leds.h"
#include "drivers/input.h"
#include "drivers/display.h"
#include "drivers/radio.h"
#include "drivers/serial_proxy.h"

#define AUTO_CONNECT


void setup() {

    led_handler.init();
    input_handler.init();
    radio.init();
    serial_proxy.init();

    //led_handler.startupBlink();
}


void loop() {
    led_handler.update();
    input_handler.update();
    serial_proxy.checkForSerialConnection();


    if (state.operation_mode == OPERATION_MODE_NORMAL) {
        //radio.update();
    } else if (state.operation_mode == OPERATION_MODE_PROXY) {
        serial_proxy.update();
    }
}