#include "drivers/serial_proxy.h"
#include "sys.h"
#include "drivers/leds.h"


typedef struct {
    uint16_t lx;
    uint16_t ly;
    uint16_t rx;
    uint16_t ry;
    uint8_t  lsw;
    uint8_t  rsw;
} joystick_packet_t;


SerialProxy::SerialProxy()
: m_t0(0)
{}

bool SerialProxy::init()
{
    SYS_SERIAL.begin(SYS_SERIAL_BAUD_RATE);
    SYS_SERIAL.setTimeout(PROXY_TIMEOUT_MS);
    SYS_SERIAL.flush();
    return true;
}

void SerialProxy::checkForSerialConnection()
{
    switch (state.operation_mode) {
        case OPERATION_MODE_NORMAL:
            if (SYS_SERIAL.available()) {
                state.operation_mode = OPERATION_MODE_PROXY;
                m_t0 = millis();
            }
            break;
        case OPERATION_MODE_PROXY:
            if ( (millis() - m_t0) > PROXY_TIMEOUT_MS) {
                state.operation_mode = OPERATION_MODE_NORMAL;
            }
            break;
    }

}

void SerialProxy::update()
{
    if (!SYS_SERIAL.readBytes(&m_cmd, 1)) {
        state.operation_mode = OPERATION_MODE_NORMAL;
        return;      // Timed out, return early.
    }

    // Update last received command.
    m_t0 = millis();

    switch (m_cmd) {
        case PROXY_COMMAND_READ:
            read();
            break;
        case PROXY_COMMAND_WRITE:
            write();
            break;
        case PROXY_COMMAND_READ_JOYSTICKS:
            sendJoySticks();
            break;
    }
}

void SerialProxy::write()
{
    // Read from Serial into transmitting packet.
    SYS_SERIAL.readBytes(m_buf_tx, PACKET_SIZE);

    // Write buffer through radio.
    radio.write(m_buf_tx, PACKET_SIZE);
}

void SerialProxy::read()
{
    // Read through radio.
    radio.read(m_buf_rx, PACKET_SIZE);

    // Sent read data through serial.
    SYS_SERIAL.write(m_buf_rx, PACKET_SIZE);
}

void SerialProxy::sendJoySticks()
{
    joystick_packet_t joystick {
        .lx = state.joystick_left.x,
        .ly = state.joystick_left.y,
        .rx = state.joystick_right.x,
        .ry = state.joystick_right.y,
        .lsw = state.joystick_left.is_pressed,
        .rsw = state.joystick_right.is_pressed
    };
    SYS_SERIAL.write((const uint8_t*) &joystick, 10);
}

SerialProxy serial_proxy;
