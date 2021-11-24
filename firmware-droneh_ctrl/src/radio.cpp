#include "drivers/radio.h"

#include "Arduino.h"

const uint8_t _address[5] = {'A', 'A', 'A', 'A', 'A'};
eeprom_t radio_settings = {
    .radio_address   = {'A', 'A', 'A', 'A', 'A'},
    .radio_power     = RF24_PA_MAX,
    .radio_speed     = RF24_2MBPS,
    .radio_channel   = 125,
    .active_protocol = PROTOCOL_DRONE,
};

Radio::Radio(State& state)
    : Driver(state), m_radio(PIN_CE, PIN_CSN)
{

}

void Radio::updateRadioSettings()
{
    //eeprom_t radio_settings = state->getEepromSettings();
    m_radio.setAddressWidth(RADIO_ADDRESS_SIZE);
    m_radio.setPALevel(radio_settings.radio_power);
    m_radio.setChannel(radio_settings.radio_channel);
    m_radio.setDataRate(radio_settings.radio_speed);
}

bool Radio::init()
{
    m_radio.setAddressWidth(5);
    m_radio.setChannel(125);
    m_radio.setDataRate(RF24_2MBPS);
    m_radio.setPALevel(RF24_PA_MAX);
    m_radio.setPayloadSize(4);
    if (!m_radio.begin())
        return false;

    m_radio.openWritingPipe(_address);
    m_radio.openReadingPipe(1, _address);
    return true;
    /*
    m_radio_state = TX;
    m_last_sent = 0;
    updateRadioSettings();
    eeprom_t radio_settings = state->getEepromSettings();
    m_radio.openWritingPipe(radio_settings.radio_address);
    m_radio.openReadingPipe(RX_PIPE, radio_settings.radio_address);
    */
}

void Radio::update()
{
    if (m_is_connected) {
        if (m_radio_state == TX) {
            // Transmitting
            m_last_sent = millis();
            m_radio_state = RX;
        } else {
            // Receiving
            uint16_t d_time = millis() - m_last_sent;
            if (d_time > RX_TIMEOUT) {
                m_retries++;
                if (m_retries >= TX_RETRIES) {
                    // Max retries reached.
                }
            } else {
                m_radio_state = TX;
            }
        }

    } else {

    }
}