#include "drivers/motors.h"
#include "state.h"

static inline void setMotor(uint8_t motor, uint16_t thrust)
{
  if (!state_estimated.is_armed)
    thrust = 0;
  analogWrite(motor, thrust);
}

Motors::Motors()
{

}

bool Motors::init()
{
  pinMode(SYS_MOTOR_1, OUTPUT);
  pinMode(SYS_MOTOR_2, OUTPUT);
  pinMode(SYS_MOTOR_3, OUTPUT);
  pinMode(SYS_MOTOR_4, OUTPUT);
  off();
  return true;
}

void Motors::update()
{
  switch (m_state) {
    case OFF:
      break;
    case ON:
      break;
    case TESTING:
      doMotorTest();
      break;
  }
}

void Motors::setThrust(const motor_thrust_t& thrust)
{
  setMotor(SYS_MOTOR_1, thrust.m1);
  setMotor(SYS_MOTOR_2, thrust.m2);
  setMotor(SYS_MOTOR_3, thrust.m3);
  setMotor(SYS_MOTOR_4, thrust.m4);
}

void Motors::off()
{
  setThrust({0, 0, 0, 0});
}

/* Private */
void Motors::doMotorTest()
{
  int MAX = 10;

  if (m_motor_test_thrust == MAX) {
    m_motor_test_thrust = 0;
    switch (motor_test_state) {
      case M1: motor_test_state = M2; break;
      case M2: motor_test_state = M3; break;
      case M3: motor_test_state = M4; break;
      case M4:
        motor_test_state = M1;
        m_state = OFF;
        return;
    }
  }

}

Motors motor_control;
