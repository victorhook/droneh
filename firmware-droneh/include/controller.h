#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sys.h"
#include "state.h"
#include "pid_controller.h"

#define CONTROL_BASIC


typedef struct {
    float roll;
    float pitch;
    float yaw;
    float thrust;
} setpoint_t;


class Controller {
    public:
        Controller();
        bool init();
        void update();
    private:
        PidController   m_pid_roll,
                        m_pid_pitch,
                        m_pid_yaw;
        motor_thrust_t  m_motor_cmd;
        setpoint_t      m_setpoint;
        void testMotors();
        uint16_t constraintMotorThrust(float thrust_estimate);
        void setpointToMotorCommand(const setpoint_t& setpoint, motor_thrust_t* cmd);
        void updatePids();
        void setMotorThrust(const motor_thrust_t motor_raw);
};


extern Controller controller;

#endif /* CONTROLLER_H */
