#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sys.h"
#include "state.h"
#include "pid_controller.h"

#define CONTROL_BASIC

typedef enum {
    CONTROL_STATE_RAW_MOTOR
} control_state_e;

class Controller {
    public:
        bool init();
        void setControlState(const control_state_e control_state);
        const control_state_e getControlState() const;
        void update(const motor_thrust_t motor_raw);
        virtual void update(const state_t target, const state_t estimated, setpoint_t* setpoint) = 0;
    private:
        void setMotorThrust(const motor_thrust_t motor_raw);
        control_state_e m_control_state;
        void calculateErr(const state_t& target, const state_t& estimated, state_t* err);
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
