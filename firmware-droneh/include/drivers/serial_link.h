#ifndef SERIAL_LINK_H
#define SERIAL_LINK_H

#include "comlink.h"
#include "sys.h"


#define READ_TIMEOUT_MS 1000


class SerialLink : public ComLink
{
    private:
        uint16_t   m_baud_rate;
        SYS_SERIAL m_serial;
    public:
        SerialLink(const uint32_t baud_rate);
        bool init() override;
    protected:
        bool read(uint8_t* buf, const size_t length) override;
        bool write(const uint8_t* buf, const size_t length) override;
};

extern SerialLink serial_link;


#endif /* SERIAL_LINK_H */
