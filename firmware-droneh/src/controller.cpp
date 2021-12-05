#include "controller.h"


pid_params_t pid_param_roll   = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_pitch  = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_yaw    = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_thrust = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};


void Controller::calculateErr(const state_t& target, const state_t& estimated, state_t* err)
{
    err->attitude.roll = target.attitude.roll - estimated.attitude.roll;
    err->attitude.pitch = target.attitude.pitch - estimated.attitude.pitch;
    err->attitude.yaw = target.attitude.yaw - estimated.attitude.yaw;
    err->thrust = target.thrust - estimated.thrust;
}

bool Controller::init()
{
    return true;
}

void Controller::setControlState(const control_state_e control_state)
{
    m_control_state = control_state;
}

const control_state_e Controller::getControlState() const
{
    return m_control_state;
}

void Controller::update(const motor_thrust_t motor_raw)
{
    setMotorThrust(motor_raw);
}

void Controller::setMotorThrust(const motor_thrust_t motor_raw)
{
    if (!state_estimated.is_armed)
        return;
    motor_control.setThrust(motor_raw);
}

/* AttitudeController */
AttitudeController::AttitudeController()
 : pid_roll(&pid_param_roll),
   pid_pitch(&pid_param_pitch),
   pid_yaw(&pid_param_yaw),
   pid_thrust(&pid_param_thrust)
{}

void AttitudeController::update(const state_t target, const state_t estimated,
                                setpoint_t* setpoint)
{
    // Set new target setpoints.
    pid_roll.setTarget(target.attitude.roll);
    pid_pitch.setTarget(target.attitude.pitch);
    pid_yaw.setTarget(target.attitude.yaw);
    pid_thrust.setTarget(target.thrust);

    // Calculate new values
    setpoint->attitude.roll   = pid_roll.update(estimated.attitude.roll);
    setpoint->attitude.pitch  = pid_pitch.update(estimated.attitude.pitch);
    setpoint->attitude.yaw    = pid_yaw.update(estimated.attitude.yaw);
    setpoint->thrust = pid_thrust.update(estimated.thrust);
}

#ifdef CONTROL_BASIC
    Controller* active_controller = (Controller*) new AttitudeController();
#endif

