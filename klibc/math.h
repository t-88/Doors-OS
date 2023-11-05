#ifndef MATH_H_
#define MATH_H_

#include "types.h"
u64 power(u64 a,u64 b);
int max_i(int a , int b);
int min_i(int a , int b);

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

int max_i(int a , int b) {
    return a > b ? a : b; 
}
int min_i(int a , int b) {
    return a > b ? b : a; 
}

#endif