#pragma once

#include "shared.h"


#define PAGE_SIZE 0x1000
#define FIRST_PAGE 0x1000
extern u32 free_mem_addr;



extern void kmemcpy(uint8_t * source, uint8_t * dest,int n_bytes);
extern u32 kmalloc(u32 size,bool aligned,u32* phy);
extern void memset(u32 *addr, u32 value, u32 nbytes);



// #ifdef MEM_IMPLEMENTAION_C
// #endif
