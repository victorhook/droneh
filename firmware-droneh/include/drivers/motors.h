#ifndef MOTORS_H
#define MOTORS_H

#include "driver.h"

#define MOTOR_FRONT_RIGHT m2
#define MOTOR_FRONT_LEFT  m1
#define MOTOR_BACK_RIGHT  m3
#define MOTOR_BACK_LEFT   m4


typedef struct {
  uint16_t m1;
  uint16_t m2;
  uint16_t m3;
  uint16_t m4;
} motor_thrust_t;


class Motors : public Driver
{
    public:
        Motors();
        bool init() override;
        void update() override;
        void setThrust(const motor_thrust_t& thrust);
        void motorTest();
        void off();
    private:
      enum {
        OFF,
        TESTING,
        ON
      } m_state;
      enum {
        M1,
        M2,
        M3,
        M4,
      } motor_test_state;
      uint16_t m_motor_test_thrust;
      void doMotorTest();
};

extern Motors motor_control;

#endif /* MOTORS_H */
