#ifndef TOF_H
#define TOF_H

#include <VL53L1X.h>

#include "drivers/sensor.h"


class ToF : public AbstractSensor
{
    public:
        ToF();
        bool init() override;
        void update() override;
    private:
        VL53L1X m_tof;
};

extern ToF tof;

#endif /* TOF_H */
