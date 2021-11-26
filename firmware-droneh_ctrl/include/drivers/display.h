#ifndef DRIVER_DISPLAY_H
#define DRIVER_DISPLAY_H

#include "driver.h"

#define SCREEN_ADDRESS 0x3C


class DisplayHandler : public Driver {
    private:
        int cursor;
    public:
        DisplayHandler(State* state);
        virtual bool init();
        virtual void update();
};


#endif /* DRIVER_DISPLAY_H */
