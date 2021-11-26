#ifndef STATE_H
#define STATE_H

#include "stdint.h"
#include "protocol.h"

#include "RF24.h"


#define RADIO_ADDRESS_SIZE 5


typedef struct {
    uint8_t          radio_address[RADIO_ADDRESS_SIZE];
    rf24_pa_dbm_e    radio_power;
    rf24_datarate_e  radio_speed;
    uint8_t          radio_channel;
    int              active_protocol;
} eeprom_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    bool is_pressed;
} joystick_t;


class State {

    private :
        eeprom_t    m_eeprom_settings;

    public:
        bool        is_connected;
        joystick_t  joystick_left;
        joystick_t  joystick_right;

        const eeprom_t& getEepromSettings();
        /* Saves settings to eeprom */
        void saveEepromSettings();

        const void getRadioAddress(char* buf);
        const void getRadioPower(char* buf);
        const void getRadioSpeed(char* buf);
        const void getRadioChannel(char* buf);
        const void getProtocol(char* buf);
};

#endif /* STATE_H */
