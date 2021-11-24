#ifndef MOTORS_H
#define MOTORS_H

#include "driver.h"
#include "state.h"


class Motors : public Driver
{
    private:
        const state_t* current_state;
    public:
        Motors(const state_t* current_state);
        bool init() override;
        void update() override;
        void setThrust();
};

extern Motors motor_control;

#endif /* MOTORS_H */
