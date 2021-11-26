#ifndef MOTORS_H
#define MOTORS_H

#include "driver.h"
#include "state.h"

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
        void setThrust(const motor_thrust_t thrust);
        void off();
};

extern Motors motor_control;

#endif /* MOTORS_H */
