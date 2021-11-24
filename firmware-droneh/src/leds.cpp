#include "drivers/leds.h"


#define LED_ON(led) analogWrite(led, 40)
#define LED_OFF(led) analogWrite(led, 0)


static void _blink_blocking()
{
  LED_ON(SYS_LED_RX);
  LED_ON(SYS_LED_STATUS);
  delay(100);
  LED_OFF(SYS_LED_RX);
  LED_OFF(SYS_LED_STATUS);
  delay(100);
}

Leds::Leds()
{
    
}

void Leds::startUpBlink() const
{
  for (int i = 0; i < 5; i++) {
    _blink_blocking();
  }
}

bool Leds::init()
{
    pinMode(SYS_LED_RX, OUTPUT);
    pinMode(SYS_LED_STATUS, OUTPUT);
    LED_OFF(SYS_LED_RX);
    LED_OFF(SYS_LED_STATUS);
    return true;
}

void Leds::update()
{

}

void Leds::setRx()
{
  LED_ON(SYS_LED_RX);
  LED_OFF(SYS_LED_STATUS);
}
void Leds::setStatus()
{
  // Tmp!
  LED_OFF(SYS_LED_RX);
  LED_ON(SYS_LED_STATUS);
}

Leds led_control;
