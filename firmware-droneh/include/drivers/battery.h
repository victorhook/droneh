#ifndef BATTERY_H
#define BATTERY_H

#include "driver.h"

#define BATTERY_MAX 4.2
#define BATTERY_MIN 3.2

class Battery : public Driver
{
    public:
        Battery();
        bool init() override;
        void update() override;
};

extern Battery battery;

#endif /* BATTERY_H */
