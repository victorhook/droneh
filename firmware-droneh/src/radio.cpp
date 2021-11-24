#include "drivers/radio.h"

#define RADIO_TIMEOUT 100

const uint8_t _address[5] = {'A', 'A', 'A', 'A', 'A'};


Radio::Radio()
: m_radio(SYS_RADIO_CE, SYS_RADIO_CSN),
  m_radio_state(RX)
{
    memcpy(m_address, _address, 5);
}

bool Radio::init()
{
    m_radio.setAddressWidth(5);
    m_radio.setChannel(125);
    m_radio.setDataRate(RF24_2MBPS);
    m_radio.setPALevel(RF24_PA_MAX);
    m_radio.setPayloadSize(sizeof(state_t));
    if (!m_radio.begin(&SYS_RADIO_SPI))
        return false;

    m_radio.openWritingPipe(m_address);
    m_radio.openReadingPipe(1, m_address);
    return true;
}


bool Radio::rx()
{
    m_radio.startListening();
    unsigned long t0 = millis();

    uint8_t pipe;
    while (!m_radio.available(&pipe) && ((millis() - t0) < RADIO_TIMEOUT)) {
        // Wait for packet to arrive.
    }

    if (pipe) {
        m_radio.read(m_buf, sizeof(state_t));
        m_radio.stopListening();
    }

    return pipe;
}

bool Radio::tx()
{
    return m_radio.write(m_buf, sizeof(state_t));
}

bool Radio::work(state_t current_state)
{
    bool result = false;
    switch (m_radio_state) {
        case RX:
            result =  rx();
            m_radio_state = RX;
            break;
        case TX:
            result = tx();
            m_radio_state = TX;
            break;
    }
    return result;
}

void Radio::update() {}

const radio_state_t Radio::getRadioState() const
{
    return m_radio_state;
}


Radio radio;
