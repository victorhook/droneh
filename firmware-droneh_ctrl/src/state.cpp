#include "state.h"
#include "string.h"

state_t state {
    .is_armed = false,
    .is_connected = false,
    .operation_mode = OPERATION_MODE_NORMAL
};


const eeprom_t& State::getEepromSettings()
{
    return m_eeprom_settings;
};

void State::saveEepromSettings()
{

};

const void State::getRadioAddress(char* buf)
{
    for (int i = 0; i < RADIO_ADDRESS_SIZE; i++) {
        buf[i] = m_eeprom_settings.radio_address[i];
    }
};

const void State::getRadioPower(char* buf)
{
    switch (m_eeprom_settings.radio_power) {
        case RF24_PA_MIN:
            strcpy(buf, "MIN");
            break;
        case RF24_PA_LOW:
            strcpy(buf, "LOW");
            break;
        case RF24_PA_HIGH:
            strcpy(buf, "HIGH");
            break;
        case RF24_PA_MAX:
            strcpy(buf, "HIGH");
            break;
        default:
            strcpy(buf, "Error");
            break;
    }
};

const void State::getRadioSpeed(char* buf)
{
    switch (m_eeprom_settings.radio_speed) {
        case RF24_1MBPS:
            strcpy(buf, "1 Mb/s");
            break;
        case RF24_2MBPS:
            strcpy(buf, "2 Mb/s");
            break;
        case RF24_250KBPS:
            strcpy(buf, "250 Kb/s");
            break;
        default:
            strcpy(buf, "Error");
            break;
    }
};

const void State::getRadioChannel(char* buf)
{
    itoa(m_eeprom_settings.radio_channel, buf, 10);
};

const void State::getProtocol(char* buf)
{

};
