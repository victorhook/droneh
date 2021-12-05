#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"


typedef struct {
    union {
        float x;
        float roll;
        float p;
    };
    union {
        float y;
        float pitch;
        float q;
    };
    union {
        float z;
        float yaw;
        float r;
    };
} vec3_t;


#endif /* TYPES_H */
