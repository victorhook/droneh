#ifndef DRIVER_H
#define DRIVER_H

#include "sys.h"


class Driver {
    public:
        virtual bool init() = 0;
        virtual void update() = 0;
};

// Macros
#define init_driver(driver, driver_code) \
    if (!driver.init()) {\
        error |= (1 << driver_code); \
    } \


#endif /* DRIVER_H */
