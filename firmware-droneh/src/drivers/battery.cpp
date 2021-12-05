#include "drivers/battery.h"

float conversion_factor = 3 * 3.3 / 65535;

Battery::Battery()
{}

bool Battery::init()
{
    pinMode(SYS_INPUT_VOLTAGE, INPUT);
    pinMode(SYS_USB_POWER, INPUT);
    return true;
}

void Battery::update()
{
    uint16_t voltage = analogRead(SYS_INPUT_VOLTAGE) * conversion_factor;
    float percentage = 100 * ( (voltage - BATTERY_MIN) / (BATTERY_MAX - BATTERY_MIN) );
}

Battery battery;
