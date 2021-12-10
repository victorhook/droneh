#ifndef FILTER_LOWASS_H
#define FILTER_LOWASS_H

#include "sys.h"


class FilterMovingAverage {
    public:
        FilterMovingAverage(const size_t order);
        float update(const float value);
    private:
        float  m_samples[MOVING_AVERAGE_FILTER_MAX_ORDER+1],
               m_h[MOVING_AVERAGE_FILTER_MAX_ORDER+1];
        size_t m_samples_taken,
               m_order;
        void shiftSamples();
};

#endif /* FILTER_LOWASS_H */
