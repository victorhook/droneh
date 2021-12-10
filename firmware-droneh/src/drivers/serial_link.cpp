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
    size_t read = m_serial.readBytes(buf, length);
    return read == length;
}

bool SerialLink::write(const uint8_t* buf, const size_t length)
{
    size_t written = m_serial.write(buf, length);
    m_serial.flush();
    return written;
}

SerialLink serial_link(SYS_SERIAL_BAUD_RATE);
