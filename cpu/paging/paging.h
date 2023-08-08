#pragma once 
#include "shared.h"
#include "isr.h"



typedef struct Page {
   u32 present    : 1;   
   u32 rw         : 1;   
   u32 user       : 1;   
   u32 accessed   : 1;   
   u32 dirty      : 1;   
   u32 unused     : 7;   
   u32 frame      : 20;   
} Page;


typedef struct PageTable {
    Page pages[1024];
} PageTable;

typedef struct PageDir {
    PageTable* tables[1024];
    u32 tables_phy[1024];
    u32 table_addr;
} PageDir;


extern void paging_init();
extern void page_dir_switch(PageDir* page_dir);
extern Page* get_page(u32 addr,bool make,PageDir* page_dir);
extern void page_fault(Intrrupt_mdata regs);
extern void paging_enable(u32 addr);

// Frame Code:
extern u32* frames;
extern u32 nframes;

#define BIT_INDEX(a) ((a)/(32))
#define BIT_OFFSET(a) ((a)%(32))