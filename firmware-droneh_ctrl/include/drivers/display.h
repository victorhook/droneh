#ifndef DRIVER_DISPLAY_H
#define DRIVER_DISPLAY_H

#include "driver.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C


class DisplayHandler : public Driver {
    public:
        DisplayHandler();
        bool init() override;
        void update() override;
    private:
        int cursor;
        Adafruit_SSD1306 m_display;
};


#endif /* DRIVER_DISPLAY_H */
