#include "drivers/radio.h"
#include "drivers/leds.h"
#include "state.h"

extern Leds led_control;
extern state_t current_state;

const uint8_t _address[5] = {'A', 'A', 'A', 'A', 'A'};


Radio::Radio()
: m_radio(SYS_RADIO_CE, SYS_RADIO_CSN),
  m_log_params_sent(0)
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
    //m_radio.setPayloadSize(sizeof(state_t));
    m_radio.setPayloadSize(4);
    m_radio.openWritingPipe(m_address);
    m_radio.openReadingPipe(1, m_address);
    return true;
}


bool Radio::rx()
{
    m_radio.startListening();

    if (m_radio.available()) {
        uint8_t buf[PACKET_SIZE];
        m_radio.read(buf, PACKET_SIZE);
        m_radio.stopListening();
        decode_packet(buf, m_packet_rx);
        led_control.blinkRx();
        respond();
    }

    return true;
}

bool Radio::tx(const packet_type_e type)
{
    m_packet_tx.protocol = PROTOCOL_IN_USE;
    m_packet_tx.version  = PROTOCOL_VERSION;
    m_packet_tx.version  = type;
    uint8_t buf[PACKET_SIZE];
    encode_packet(buf, m_packet_tx);
    m_radio.write(buf, PACKET_SIZE);
    return true;
}

bool Radio::respond()
{
    switch (m_packet_rx.type) {
        case PACKET_TYPE_RAW_MOTOR_THRUST:
            break;
        case PACKET_TYPE_SETPOINT_ATTITUDE:
            break;
        case PACKET_TYPE_LOGGING_DOWNLOAD:
            tx(PACKET_TYPE_LOGGING_DOWNLOAD_ACK);
            break;
        case PACKET_TYPE_LOGGING_DOWNLOAD_ACK:
            break;
    }
    return true;
}

void Radio::update() {
    rx();
}


Radio radio;
