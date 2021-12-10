#include "filter_lowpass.h"


FilterMovingAverage::FilterMovingAverage(const size_t order)
: m_order(order)
{
    memset(m_samples, 0, order);
    memset(m_h, 1/order, order);
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

    if (m_samples_taken < m_order) {
        m_samples_taken++;
        return value;
    }

    float res = 0;
    for (int i = 0; i <= m_order; i++) {
        res += m_samples[i] / (m_order + 1);
    }
    return res;
}
