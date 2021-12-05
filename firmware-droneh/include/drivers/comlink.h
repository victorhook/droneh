#ifndef COM_LINK_H
#define COM_LINK_H

#include "driver.h"
#include "protocol.h"
#include "log.h"

#define DISCONNECT_TIMEOUT_MS 1000


class ComLink : public Driver
{
    public:
        ComLink();
        void update();
        bool isConnected();
        virtual bool init() = 0;

    protected:
        // Abstact methods
        virtual bool read(uint8_t* buf, const size_t length) = 0;
        virtual bool write(const uint8_t* buf, const size_t length) = 0;

    private:
        bool          m_connected;
        unsigned long m_last_packet;
        size_t        m_log_params_sent;
        com_state_e   m_state;
        packet_t      m_packet_rx,
                      m_packet_tx;

        bool transmit(const packet_type_e type = PACKET_TYPE_ACK);
        void fillPacketAvailableLogParam(const log_param_t& log_param);
        size_t fillPacketLogParam(const log_param_t& log_param, const uint8_t pkt_pos);
        void fillPacketLogParams(const log_param_id_t param_ids[],
                                const size_t nbr_of_params);
        void setRawMotorSetpoints();
        void sendStatusUpdate();
        void sendLogParameters();
        void sendAvailableLogParam();
        bool receive();
        bool respond();
};


#endif /* COM_LINK_H */
