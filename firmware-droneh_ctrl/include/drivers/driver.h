#ifndef DRIVER_H
#define DRIVER_H

#include "sys.h"
#include "state.h"


class Driver {
    protected:
        State* m_state;

    public:
        Driver(State* state);
        virtual bool init() = 0;
        virtual void update() = 0;
};


#endif /* DRIVER_H */
