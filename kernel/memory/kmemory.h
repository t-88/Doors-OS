#ifndef MEMORY_H_
#define MEMORY_H_
#include "shared.h"

void mem_copy(uint8_t * source, uint8_t * dest,int n_bytes) {
    if(n_bytes < 0) {
        return;
    }
    for (int i = 0; i < n_bytes; i++) {
        *(dest + i) = *(source + i); 
    }
    
}

#endif