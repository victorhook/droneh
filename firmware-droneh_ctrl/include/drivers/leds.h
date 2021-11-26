#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#include "driver.h"


class LedHandler : public Driver {
    public:
        LedHandler(State* state);
        virtual bool init();
        virtual void update();
};

#endif /* DRIVER_LED_H */
