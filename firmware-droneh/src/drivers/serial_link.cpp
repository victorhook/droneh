#include "drivers/serial_link.h"

SerialLink::SerialLink(const uint32_t baud_rate)
 : ComLink(),
   m_baud_rate(m_baud_rate)
{}

bool SerialLink::init()
{
    m_serial.begin(m_baud_rate);
    m_serial.setTimeout(READ_TIMEOUT_MS);
    return true;
}

bool SerialLink::read(uint8_t* buf, const size_t length)
{
    return m_serial.readBytes(buf, length) == length;
}

bool SerialLink::write(const uint8_t* buf, const size_t length)
{
    return m_serial.write(buf, length);
}

SerialLink serial_link(SYS_SERIAL_BAUD_RATE);
