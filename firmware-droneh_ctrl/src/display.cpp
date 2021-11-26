#include "drivers/display.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define MAX_ROWS 5
#define NBR_OF_SETTINGS 9

static Adafruit_SSD1306 display(128, 64, &Wire);

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


DisplayHandler::DisplayHandler(State* state)
    : Driver(state)
{

}

bool DisplayHandler::init()
{
    return true;
}

void DisplayHandler::update()
{
    /*
    display.clearDisplay();
    char buf[15];
    eeprom_t settings = state->getEepromSettings();
    */
    //display.write("> Address: ");  state->getRadioAddress(buf);  display.write(buf);
}