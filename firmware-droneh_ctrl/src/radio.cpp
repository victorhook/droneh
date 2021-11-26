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

Radio::Radio(State* state)
    : Driver(state),
      m_radio(RADIO_CE, RADIO_CSN),
      m_radio_state(TX)
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
    if (!m_radio.begin())
        return false;

    m_radio.setAddressWidth(5);
    m_radio.setChannel(125);
    m_radio.setDataRate(RF24_2MBPS);
    m_radio.setPALevel(RF24_PA_MAX);
    m_radio.setPayloadSize(4);

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

bool Radio::rx()
{
    m_radio.startListening();
    uint8_t pipe;

    if (!m_radio.available(&pipe)) {
        m_radio.read(m_buf, 4);
        m_radio.stopListening();
    }

    return pipe;
}

#include "state.h"
extern State state;

static uint8_t convert(int val) {
    int x = val - 512;

    if (x < 10)
        x = 0;

    x = map(x, 0, 1023, 0, 255);
    x = x << 1;
    
    Serial.print("  "); Serial.println(x);
    return x;
}

bool Radio::tx()
{
    uint8_t pkt[4] = {
        convert(m_state->joystick_left.x),
        convert(m_state->joystick_left.y),
        convert(m_state->joystick_right.x),
        convert(m_state->joystick_right.y),
    };
    //Serial.print(pkt[0]);
    //Serial.print(pkt[1]);
    //Serial.print(pkt[2]);
    //Serial.print(pkt[3]);
    //Serial.println("");
    bool report = m_radio.write(pkt, 4);
    if (report) {
        //Serial.println("Success");
    }
    return report;
}

void Radio::update()
{
    if (m_radio_state == RX) {
        rx();
        m_radio_state = TX;
    } else {
        tx();
        m_radio_state = RX;
    }



    return;
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

const radio_state_e Radio::getRadioState() const
{
    return m_radio_state;
}