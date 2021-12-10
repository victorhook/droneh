#include "drivers/radio.h"
#include "sys.h"


const uint8_t _address[5] = {'A', 'A', 'A', 'A', 'A'};
eeprom_t radio_settings = {
    .radio_address   = {'A', 'A', 'A', 'A', 'A'},
    .radio_power     = RF24_PA_MAX,
    .radio_speed     = RF24_2MBPS,
    .radio_channel   = 125,
    .active_protocol = PACKET_PROTOCOL_DRONE,
};

Radio::Radio()
: m_radio(RADIO_CE, RADIO_CSN)
{}

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
    m_radio.setPayloadSize(32);
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

/* Low level reading of data */
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

/* Low level writing of data */
bool Radio::write(const uint8_t* buf, const size_t length)
{
    return m_radio.write(buf, length);
}

/* Higher level reading of packet */
bool Radio::receive()
{
    uint8_t buf[PACKET_SIZE];
    if (!read(buf, PACKET_SIZE)) {
        return false;
    }
    m_last_packet = millis();
    decode_packet(buf, m_packet_rx);
    return true;
}

/* Higher level writing of packet */
bool Radio::transmit(const packet_type_e type)
{
    m_packet_tx.protocol = PROTOCOL_IN_USE;
    m_packet_tx.version  = PROTOCOL_VERSION;
    m_packet_tx.type     = type;

    uint8_t buf[PACKET_SIZE];
    encode_packet(buf, m_packet_tx);
    int res = write(buf, PACKET_SIZE);
    //Serial.print("Transmitted: "); Serial.println(res);
    return true;
}

bool Radio::respond()
{
    return false;
}

void Radio::tryToConnect()
{
    transmit(PACKET_TYPE_CONNECT_REQUEST);
    if (!receive())
        return;
    state.is_connected = m_packet_rx.type == PACKET_TYPE_CONNECT_REQUEST_ACK;
    //Serial.print(m_packet_rx.type); Serial.print(" ");
    //Serial.print(m_packet_rx.type == PACKET_TYPE_CONNECT_REQUEST_ACK); Serial.print(" ");
    //Serial.println(state.is_connected);
}

void Radio::disconnect()
{
    state.is_connected = false;
    // Disarm on disconnection, to prevent too much chaos!
    state.is_armed = false;
}

void Radio::update()
{
    if (state.is_connected && ((millis() - m_last_packet) > DISCONNECT_TIMEOUT_MS) ) {
        // If we're connected and timeout occurs, we break the connection.
        disconnect();
    }

    if (!state.is_connected) {
        tryToConnect();
    }

    transmit(PACKET_TYPE_STATUS_UPDATE);
    receive();
}



Radio radio;
