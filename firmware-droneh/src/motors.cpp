#include "drivers/motors.h"


Motors::Motors(const state_t* current_state)
    : current_state(current_state)
{

}

bool Motors::init()
{
  pinMode(SYS_MOTOR_1, OUTPUT);
  pinMode(SYS_MOTOR_2, OUTPUT);
  pinMode(SYS_MOTOR_3, OUTPUT);
  pinMode(SYS_MOTOR_4, OUTPUT);
  analogWrite(SYS_MOTOR_1, 0);
  analogWrite(SYS_MOTOR_2, 0);
  analogWrite(SYS_MOTOR_3, 0);
  analogWrite(SYS_MOTOR_4, 0);
  return true;
}

void Motors::update()
{

}

void Motors::setThrust()
{
    analogWrite(SYS_MOTOR_1, current_state->thrust);
    analogWrite(SYS_MOTOR_2, current_state->thrust);
    analogWrite(SYS_MOTOR_3, current_state->thrust);
    analogWrite(SYS_MOTOR_4, current_state->thrust);
}

extern state_t current_state;
Motors motor_control(&current_state);
