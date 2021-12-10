#include "controller.h"


pid_params_t pid_param_roll   = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_pitch  = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};
pid_params_t pid_param_yaw    = {.Kp = 1, .Ki = 1, .Kd = 1, .target = 0, .outMin = 1, .outMax = 1};


Controller::Controller()
: m_pid_roll(&pid_param_roll),
  m_pid_pitch(&pid_param_pitch),
  m_pid_yaw(&pid_param_yaw)
{}

bool Controller::init()
{
    return true;
}

void Controller::update()
{
    switch (state_estimated.mode) {
        case OPERATING_MODE_MOTOR_THRUST_RAW:
            m_motor_cmd.m1 = state_target.motor_raw.m1;
            m_motor_cmd.m2 = state_target.motor_raw.m2;
            m_motor_cmd.m3 = state_target.motor_raw.m3;
            m_motor_cmd.m4 = state_target.motor_raw.m4;
            motor_control.setThrust(m_motor_cmd);
            break;
        case OPERATING_MODE_NORMAL_CONTROL:
            // 1. Update PID-controllers.
            m_setpoint.roll = m_pid_roll.update(state_estimated.attitude.roll, state_target.attitude.roll);
            m_setpoint.pitch = m_pid_pitch.update(state_estimated.attitude.pitch, state_target.attitude.pitch);
            m_setpoint.yaw = m_pid_yaw.update(state_estimated.attitude.yaw, state_target.attitude.yaw);

            // 2. Convert setpoint to motor commands
            setpointToMotorCommand(m_setpoint, &m_motor_cmd);

            // 3. Done
            motor_control.setThrust(m_motor_cmd);
            break;
    }
}

uint16_t Controller::constraintMotorThrust(float thrust_estimate)
{
    return constrain(thrust_estimate, 0, 0xffff);
}

/*
    https://www.youtube.com/watch?v=hGcGPUqB67Q&t=2s
*/
void Controller::setpointToMotorCommand(const setpoint_t& setpoint, motor_thrust_t* cmd)
{
    cmd->MOTOR_FRONT_RIGHT = constraintMotorThrust(setpoint.thrust + setpoint.yaw + setpoint.pitch + setpoint.roll);
    cmd->MOTOR_FRONT_LEFT  = constraintMotorThrust(setpoint.thrust - setpoint.yaw + setpoint.pitch - setpoint.roll);
    cmd->MOTOR_BACK_RIGHT  = constraintMotorThrust(setpoint.thrust - setpoint.yaw - setpoint.pitch + setpoint.roll);
    cmd->MOTOR_BACK_LEFT   = constraintMotorThrust(setpoint.thrust + setpoint.yaw - setpoint.pitch - setpoint.roll);
}

void Controller::updatePids()
{
    // Set new target setpoints.
    m_pid_roll.setTarget(state_target.attitude.roll);
    m_pid_pitch.setTarget(state_target.attitude.pitch);
    m_pid_yaw.setTarget(state_target.attitude.yaw);

    // Calculate new values
    m_setpoint.roll   = m_pid_roll.update(state_estimated.attitude.roll);
    m_setpoint.pitch  = m_pid_pitch.update(state_estimated.attitude.pitch);
    m_setpoint.yaw    = m_pid_yaw.update(state_estimated.attitude.yaw);

}

void Controller::setMotorThrust(const motor_thrust_t motor_raw)
{
    if (state_estimated.is_armed)
        motor_control.setThrust(motor_raw);
    else
        motor_control.off();
}



Controller controller;
