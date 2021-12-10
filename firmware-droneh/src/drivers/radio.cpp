#include "drivers/radio.h"

#define RX_TIMEOUT 10
const uint8_t _address[5] = {'A', 'A', 'A', 'A', 'A'};


Radio::Radio()
 : ComLink(),
   m_radio(SYS_RADIO_CE, SYS_RADIO_CSN)
{
    memcpy(m_address, _address, 5);
}

bool Radio::init()
{
    if (!m_radio.begin(&SYS_RADIO_SPI))
        return false;
    m_radio.setAddressWidth(5);
    m_radio.setChannel(125);
    m_radio.setDataRate(RF24_2MBPS);
    m_radio.setPALevel(RF24_PA_MAX);
    m_radio.setPayloadSize(32);
    m_radio.openWritingPipe(m_address);
    m_radio.openReadingPipe(1, m_address);
    return true;
}


bool Radio::read(uint8_t* buf, const size_t length)
{
    bool success = false;
    m_radio.startListening();
    unsigned long t0 = millis();
    while ((millis() - t0) < RX_TIMEOUT) {
        if (m_radio.available()) {
            m_radio.read(buf, length);
            success = true;
        }
    }

    m_radio.stopListening();
    return success;
}

bool Radio::write(const uint8_t* buf, const size_t length)
{
    return m_radio.write(buf, length);
}


Radio radio;
