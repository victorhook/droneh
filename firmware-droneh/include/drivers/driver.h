#ifndef DRIVER_H
#define DRIVER_H

#include "sys.h"


class Driver {
    public:
        virtual bool init() = 0;
        virtual void update() = 0;
};


#endif /* DRIVER_H */
