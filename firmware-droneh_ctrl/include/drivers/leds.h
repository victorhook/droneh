#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#include "driver.h"


class LedHandler : public Driver {
    public:
        LedHandler();
        bool init() override;
        void startupBlink();
        void update() override;
};

extern LedHandler led_handler;

#endif /* DRIVER_LED_H */
