#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H


typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float target;
    float outMin;
    float outMax;
} pid_params_t;


class PidController {

    private:
        pid_params_t* params;
        float p;
        float i;
        float d;
        float last_error;
        float last_time;

    public:
        PidController(pid_params_t* params);
        void setTarget(float new_target);
        float update(float value);
};

#endif /* PID_CONTROLLER_H */
