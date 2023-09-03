#pragma once

#include "shared.h"


#define PAGE_SIZE 0x1000
#define FIRST_PAGE 0x1000
extern u32 free_mem_addr;



extern void kmemcpy(uint8_t * source, uint8_t * dest,int n_bytes);
extern u32 kmalloc(u32 size,bool aligned,u32* phy);
extern void memset(u32 *addr, u32 value, u32 nbytes);




#ifdef MEM_IMPLEMENTAION_C


u32 free_mem_addr = 2*FIRST_PAGE;

void kmemcpy(u8 * source, u8 * dest,int n_bytes) {
    if(n_bytes < 0) {
        return;
    }
    for (int i = 0; i < n_bytes; i++) {
        *(dest + i) = *(source + i); 
    }
}

u32 kmalloc(u32 size,bool aligned, u32* phy) {
    if(aligned == 1 && (free_mem_addr & 0x00000FFF)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;

    }

    

    if(phy) {
        *phy = free_mem_addr;
    }

    u32 addr = free_mem_addr;
    free_mem_addr += size; 
    return addr;
}


void memset(u32 *addr, u32 value, u32 nbytes) {
    u32 *temp = (u32*)addr;
    for ( ; nbytes != 0; nbytes--) *temp++ = value;
}
#endif
