#ifndef FILTER_LOWASS_H
#define FILTER_LOWASS_H

#define MAX_MOVING_AVERAGE_FILTER_LENGTH 128
#include "sys.h"


class FilterMovingAverage {
    public:
        FilterMovingAverage(const size_t order);
        float update(const float value);
    private:
        float  m_samples[MAX_MOVING_AVERAGE_FILTER_LENGTH];
        size_t m_order;
        void shiftSamples();
};

#endif /* FILTER_LOWASS_H */
