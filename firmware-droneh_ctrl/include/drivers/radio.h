#ifndef DRIVER_RADIO_H
#define DRIVER_RADIO_H

#include "driver.h"
#include "state.h"
#include "protocol.h"
#include "RF24.h"


#define PIN_MOSI 1
#define PIN_MISO 1
#define PIN_SCK  1
#define PIN_CE   7
#define PIN_CSN  8

#define RX_TIMEOUT 1000
#define RX_PIPE    1
#define TX_RETRIES 3


typedef enum {
    TX,
    RX
} radio_state_e;


class Radio : public Driver {
    private:
        Protocol      protocol;
        bool          m_is_connected;
        uint16_t      m_last_sent;
        uint16_t      m_retries;
        radio_state_e m_radio_state;
        RF24          m_radio;


    public:
        Radio(State& state);
        void updateRadioSettings();
        virtual bool init();
        virtual void update();
};


#endif /* DRIVER_RADIO_H */
