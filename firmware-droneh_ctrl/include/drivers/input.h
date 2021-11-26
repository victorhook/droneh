#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H

#include "driver.h"


class InputHandler : public Driver {
    public:
        InputHandler(State* state);
        virtual bool init();
        virtual void update();
};


#endif /* DRIVER_INPUT_H */
