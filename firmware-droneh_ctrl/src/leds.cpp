#include "drivers/leds.h"

#define LED_OFF(led) analogWrite(led, 0)
#define LED_ON(led)  analogWrite(led, 70)

LedHandler::LedHandler(State* state)
    : Driver(state)
{

}

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

void LedHandler::update()
{

}