#ifndef SERIAL_PROXY_H
#define SERIAL_PROXY_H

#include "drivers/driver.h"
#include "drivers/radio.h"
#include "state.h"

#define PROXY_TIMEOUT_MS 500

typedef enum {
    PROXY_COMMAND_READ,
    PROXY_COMMAND_WRITE,
    PROXY_COMMAND_READ_JOYSTICKS,
} proxy_command_e;


class SerialProxy : public Driver {
    public:
        SerialProxy();
        bool init() override;
        void update() override;
        void checkForSerialConnection();
    private:
        unsigned long m_t0;
        uint8_t       m_buf_rx[PACKET_SIZE],
                      m_buf_tx[PACKET_SIZE],
                      m_cmd;
        void sendJoySticks();
        void read();
        void write();
};


extern SerialProxy serial_proxy;

#endif /* SERIAL_PROXY_H */
