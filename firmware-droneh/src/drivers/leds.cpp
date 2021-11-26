#include "drivers/leds.h"


#define LED_ON(led)  digitalWrite(led, HIGH)
#define LED_OFF(led) digitalWrite(led, LOW)


static inline void _blink_blocking()
{
  LED_ON(SYS_LED_RX);
  LED_ON(SYS_LED_STATUS);
  delay(100);
  LED_OFF(SYS_LED_RX);
  LED_OFF(SYS_LED_STATUS);
  delay(100);
}

static inline void updateBlink(blink_t blink)
{
  unsigned long t1 = millis();
  if (blink.active) {
    if ((t1 - blink.t0) > blink.delay_ms) {
      blink.active = false;
      LED_OFF(blink.led);
    }
  }
}

static inline void setBlink(blink_t blink, const size_t delay_ms)
{
  if (blink.active)
    return;
  blink.t0 = millis();
  blink.delay_ms = delay_ms;
  LED_ON(blink.led);
}

Leds::Leds()
  : m_blinkRx({.active = false, .led = SYS_LED_RX}),
    m_blinkStatus({.active = false, .led = SYS_LED_STATUS})
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
  updateBlink(m_blinkRx);
  updateBlink(m_blinkStatus);
}

void Leds::blinkRx(const size_t delay_ms)
{
  setBlink(m_blinkRx, delay_ms);
}

void Leds::blinkStatus(size_t delay_ms)
{
  setBlink(m_blinkStatus, delay_ms);
}

void Leds::off()
{
  LED_OFF(SYS_LED_RX);
  LED_OFF(SYS_LED_STATUS);
}

Leds led_control;
