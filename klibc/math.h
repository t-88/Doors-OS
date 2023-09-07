#ifndef MATH_H_
#define MATH_H_

#include "types.h"
u64 power(u64 a,u64 b);

#endif

#ifdef MATH_IMPLEMENTATION_C
#include "types.h"
u64 power(u64 a,u64 b) {
    u64 tmp = 1;
    for (u64 i = 0; i < b; i++) {
        tmp = a * tmp;
    }
    return tmp;    
}
#endif