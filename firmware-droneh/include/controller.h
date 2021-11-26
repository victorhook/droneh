#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sys.h"
#include "state.h"
#include "pid_controller.h"

#define CONTROL_BASIC


class Controller {
    private:
        void calculateErr(const state_t& target, const state_t& estimated, state_t* err);
    public:
        bool init();
        virtual void update(const state_t target, const state_t estimated, setpoint_t* setpoint) = 0;
};


class AttitudeController : public Controller {
    private:
        PidController pid_roll;
        PidController pid_pitch;
        PidController pid_yaw;
        PidController pid_thrust;
    public:
        AttitudeController();
        void update(const state_t target, const state_t estimated, setpoint_t* setpoint) override;
};

extern Controller* active_controller;

#endif /* CONTROLLER_H */
