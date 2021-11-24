#include "drivers/leds.h"


LedHandler::LedHandler(State& state)
    : Driver(state)
{

}

bool LedHandler::init()
{
    return true;
}

void LedHandler::update()
{

}