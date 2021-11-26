#include "drivers/motors.h"


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

}

void Motors::setThrust(const motor_thrust_t thrust)
{
  analogWrite(SYS_MOTOR_1, thrust.m1);
  analogWrite(SYS_MOTOR_2, thrust.m2);
  analogWrite(SYS_MOTOR_3, thrust.m3);
  analogWrite(SYS_MOTOR_4, thrust.m4);
}

void Motors::off()
{
  setThrust({0, 0, 0, 0});
}

Motors motor_control;
