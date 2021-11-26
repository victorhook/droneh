#include "drivers/baro.h"
#include "sys.h"

Baro::Baro()
: m_baro()
{

}

bool Baro::init()
{
    return m_baro.begin(BMP085_ULTRAHIGHRES, &SYS_I2C);
}

void Baro::update()
{

}

Baro baro;
