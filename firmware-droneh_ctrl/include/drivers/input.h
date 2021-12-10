#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H

#include "driver.h"


class InputHandler : public Driver {
    public:
        InputHandler();
        bool init() override;
        void update() override;
    private:
        void calibrate();
        void read();
        bool m_is_calibrated;
        int  m_idle_limit;
        struct {
            int rx;
            int ry;
            int lx;
            int ly;
        } m_bias;
        struct {
            int rx;
            int ry;
            int lx;
            int ly;
        } m_reading;
};

extern InputHandler input_handler;


#endif /* DRIVER_INPUT_H */
