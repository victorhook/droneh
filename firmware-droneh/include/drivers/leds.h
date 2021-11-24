#ifndef LEDS_H
#define LEDS_H

#include "driver.h"


class Leds : public Driver
{
    public:
        Leds();
        bool init() override;
        void update() override;
        void startUpBlink() const;
        void setRx();
        void setStatus();
};

extern Leds led_control;

#endif /* LEDS_H */
