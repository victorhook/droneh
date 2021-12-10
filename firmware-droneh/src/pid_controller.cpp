#include "sys.h"
#include "pid_controller.h"


PidController::PidController(pid_params_t* params)
: params(params),
  i(0),
  last_time(0),
  last_error(0)
{}


void PidController::setTarget(float new_target)
{
    params->target = new_target;
}

float PidController::update(float estimate, float target)
{
    setTarget(target);
    return update(estimate);
}

float PidController::update(float estimate)
{
    float curr_time = millis();
    float dt = curr_time - last_time;

    float error = params->target - estimate;

    // Proportional
    p = params->Kp * error;

    // Integral
    i += i * params->Ki * error;

    // Derivative
    d = params->Kd * ( (error - last_error) / dt );

    float value = p + i + d;

    // Constrain value within boundaries
    value = constrain(value, params->outMin, params->outMax);

    last_error = error;
    last_time  = curr_time;
    return value;
}
