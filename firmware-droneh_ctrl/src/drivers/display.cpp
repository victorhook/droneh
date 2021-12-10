#include "drivers/display.h"

#include <Wire.h>


#define MAX_ROWS 5
#define NBR_OF_SETTINGS 9


const char* messages[9] = {
    "Address",
    "Channel",
    "Data rate",
    "Power",
    "Protocol",
    "Left",
    "Left press",
    "Right",
    "Right press"
};


DisplayHandler::DisplayHandler()
: m_display(128, 64, &Wire)
{}

bool DisplayHandler::init()
{
    return m_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void DisplayHandler::update()
{
    m_display.println("Hello world");
    m_display.display();
    /*
    display.clearDisplay();
    char buf[15];
    eeprom_t settings = state->getEepromSettings();
    */
    //display.write("> Address: ");  state->getRadioAddress(buf);  display.write(buf);
}
