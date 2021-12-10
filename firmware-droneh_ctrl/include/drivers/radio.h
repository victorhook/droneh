#ifndef DRIVER_RADIO_H
#define DRIVER_RADIO_H

#include "driver.h"
#include "state.h"
#include "protocol.h"
#include "RF24.h"


#define RX_TIMEOUT 10
#define RX_PIPE    1
#define TX_RETRIES 3


class Radio : public Driver {
    public:
        Radio();
        bool init() override;
        void update() override;
        bool read(uint8_t* buf, const size_t length);
        bool write(const uint8_t* buf, const size_t length);
    private:
        unsigned long m_last_packet;
        packet_t      m_packet_rx,
                      m_packet_tx;
        RF24          m_radio;
        bool transmit(const packet_type_e type);
        bool receive();
        bool respond();
        void disconnect();
        bool sendArm(const bool is_armed);
        void setSetpoint();
        void setRawMotorSetpoints();
        void tryToConnect();

        /* Future */
        void updateRadioSettings();
};

extern Radio radio;

#endif /* DRIVER_RADIO_H */
