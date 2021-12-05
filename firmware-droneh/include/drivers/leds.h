#ifndef LEDS_H
#define LEDS_H

#include "driver.h"

typedef struct {
    unsigned long t0;
    bool         active;
    size_t       delay_ms;
    uint8_t      led;
} blink_t;


class Leds : public Driver
{
    private:
        blink_t m_blinkRx;
        blink_t m_blinkStatus;
    public:
        Leds();
        bool init() override;
        void update() override;
        void startUpBlink() const;
        void setRx(const uint8_t val);
        void setStatus(const uint8_t val);
        void blinkRx(const size_t delay_ms = 50);
        void blinkStatus(const size_t delay_ms = 50);
        void error();
        void off();
};

extern Leds led_control;

#endif /* LEDS_H */
