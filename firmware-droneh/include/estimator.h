#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "state.h"


class Estimator {
    public:
        static state_t estimate(const state_t measured);
};

#endif /* ESTIMATOR_H */
