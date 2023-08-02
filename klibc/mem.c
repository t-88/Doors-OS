#include "mem.h"


u32 free_mem_addr = 2*FIRST_PAGE;

void kmemcpy(u8 * source, u8 * dest,int n_bytes) {
    if(n_bytes < 0) {
        return;
    }
    for (int i = 0; i < n_bytes; i++) {
        *(dest + i) = *(source + i); 
    }
}

//TODO: this is no mem managment brother 
u32 kmalloc(u32 size) {
    u32 addr = free_mem_addr;
    free_mem_addr += size; 
    return addr;
}
