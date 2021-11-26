#ifndef RADIO_H
#define RADIO_H

#include "RF24.h"

#include "driver.h"
#include "sys.h"
#include "state.h"
#include "protocol.h"



class Radio : public Driver
{
    private:
        RF24           m_radio;
        uint8_t        m_address[5];
        packet_t       m_packet_rx,
                       m_packet_tx;
        size_t         m_log_params_sent;
        bool rx();
        bool tx(const packet_type_e type = PACKET_TYPE_ACK);
        bool respond();
    public:
        Radio();
        bool init() override;
        void update() override;
};

extern Radio radio;

#endif /* RADIO_H */
