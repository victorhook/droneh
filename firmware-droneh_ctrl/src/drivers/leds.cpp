#include "drivers/leds.h"
#include <sys.h>
#include "state.h"


#define LED_OFF(led) analogWrite(led, 0)
#define LED_ON(led)  analogWrite(led, 70)
#define SET_LED(led, on) on ? LED_ON(led) : LED_OFF(led)

// 1 - Green
// 2 - Pink
// 3 - Blue

static inline void off()
{
    LED_OFF(LED_BLUE);
    LED_OFF(LED_GREEN);
    LED_OFF(LED_PINK);
}

static inline void runningLight(int delay_ms = 50)
{
    off();
    LED_ON(LED_GREEN);
    delay(delay_ms);
    off();
    LED_ON(LED_PINK);
    delay(delay_ms);
    off();
    LED_ON(LED_BLUE);
    delay(delay_ms);
    off();
}


LedHandler::LedHandler()
{}

bool LedHandler::init()
{
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_PINK, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    LED_OFF(LED_BLUE);
    LED_OFF(LED_PINK);
    LED_OFF(LED_GREEN);
    return true;
}

void LedHandler::startupBlink()
{
    for (int i = 0; i < 3; i++) {
        runningLight();
    }
}

void LedHandler::update()
{
    //SET_LED(LED_PINK, state.is_armed);
    SET_LED(LED_PINK, state.operation_mode == OPERATION_MODE_PROXY);
    SET_LED(LED_BLUE, state.is_connected);
    LED_ON(LED_GREEN);
}

extern LedHandler led_handler;