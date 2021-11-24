#ifndef TOF_H
#define TOF_H

#include <VL53L1X.h>

#include "driver.h"
#include "sys.h"


class ToF : public Driver
{
    private:
        VL53L1X m_tof;
    public:
        ToF();
        bool init() override;
        void update() override;
};

extern ToF tof;

#endif /* TOF_H */
