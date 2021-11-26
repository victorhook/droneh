#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Arduino.h"

#include "drivers/driver.h"
#include "droneh_ctrl.h"


class Debugger : public Driver {
    private:
        size_t m_time_delay;

    public:
        Debugger(State* state, size_t time_delay);
        virtual bool init();
        virtual void update();
};


#endif /* DEBUGGER_H */
