#ifndef RADIO_H
#define RADIO_H

#include "RF24.h"

#include "comlink.h"


class Radio : public ComLink
{
    public:
        Radio();
        bool init() override;
    private:
        RF24           m_radio;
        uint8_t        m_address[5];
    protected:
        bool read(uint8_t* buf, const size_t length) override;
        bool write(const uint8_t* buf, const size_t length) override;
};

extern Radio radio;


#endif /* RADIO_H */
