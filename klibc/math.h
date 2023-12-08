#ifndef MATH_H_
#define MATH_H_

#include "types.h"
u64 power(u64 a,u64 b);
int max_i(int a , int b);
int min_i(int a , int b);
bool AABB(int x1,int y1,int  w1,int  h1,int  x2,int  y2,int  w2,int  h2);

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


bool AABB(int x1,int y1,int  w1,int  h1,int  x2,int  y2,int  w2,int  h2) {
    return x1 + w1 > x2 && y1 + h1 > y2 && x2 + w2 > x1 && y2 + h2 > y1; 
}

#endif