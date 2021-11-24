#ifndef RADIO_H
#define RADIO_H

#include "RF24.h"

#include "driver.h"
#include "sys.h"
#include "state.h"


typedef enum {
    PACKET_TYPE_CONTROL,
    PACKET_TYPE_INFO,
} packet_type_t;

typedef enum {
    RX,
    TX
} radio_state_t;

typedef struct {
    uint8_t protocol;
    packet_type_t type;
    uint8_t* payload;
} packet_t;


class Radio : public Driver
{
    private:
        RF24          m_radio;
        uint8_t       m_address[5];
        radio_state_t m_radio_state;
        uint8_t       m_buf[sizeof(state_t)];
        bool rx();
        bool tx();
    public:
        Radio();
        bool init() override;
        void update() override;
        bool start();
        bool work(state_t current_state);
        const radio_state_t getRadioState() const;
};

extern Radio radio;

#endif /* RADIO_H */
