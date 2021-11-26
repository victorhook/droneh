#include "controller.h"


pid_params_t pid_param_roll   = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_pitch  = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_yaw    = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_thrust = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};


void Controller::calculateErr(const state_t& target, const state_t& estimated, state_t* err)
{
    err->roll = target.roll - estimated.roll;
    err->pitch = target.pitch - estimated.pitch;
    err->yaw = target.yaw - estimated.yaw;
    err->thrust = target.thrust - estimated.thrust;
}

bool Controller::init()
{
    return true;
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
    pid_roll.setTarget(target.roll);
    pid_pitch.setTarget(target.pitch);
    pid_yaw.setTarget(target.yaw);
    pid_thrust.setTarget(target.thrust);

    // Calculate new values
    setpoint->roll   = pid_roll.update(estimated.roll);
    setpoint->pitch  = pid_pitch.update(estimated.pitch);
    setpoint->yaw    = pid_yaw.update(estimated.yaw);
    setpoint->thrust = pid_thrust.update(estimated.thrust);
}

#ifdef CONTROL_BASIC
    Controller* active_controller = new AttitudeController();
#endif

