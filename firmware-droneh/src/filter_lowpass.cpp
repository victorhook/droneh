#include "filter_lowpass.h"

FilterMovingAverage::FilterMovingAverage(const size_t order)
 : m_order(order)
{
    assert(order < MAX_MOVING_AVERAGE_FILTER_LENGTH);
}

void FilterMovingAverage::shiftSamples()
{
    for (int i = m_order; i > 0; i--) {
        m_samples[i] = m_samples[i-1];
    }
}


float FilterMovingAverage::update(const float value)
{
    shiftSamples();
    m_samples[0] = value;
    float res = 0;
    for (int i = 0; i <= m_order; i++) {
        res += m_samples[i];
    }
    res /= (m_order + 1);
    return res;
}
