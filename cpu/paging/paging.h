#include "shared.h"
#include "mem.h"
#include "isr.h"

#define BIT_INDEX(a) ((a)/(32))
#define BIT_OFFSET(a) ((a)%(32))

#ifndef PAGING_H_
#define PAGING_H_



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


#endif

// #define PAGING_IMPLEMENTATION_C
#ifdef PAGING_IMPLEMENTATION_C

u32* frames;
u32 nframes;


static void frame_set(u32 frame_addr) {
    u32 frame = frame_addr / 0x1000;
    u32 idx = BIT_INDEX(frame);
    u32 offset = BIT_OFFSET(frame);

    frames[idx] |=  (0x1 << offset);
}

// static void frame_clear(u32 frame_addr) {
    // u32 frame = frame_addr / 0x1000;
    // u32 idx = BIT_INDEX(frame);
    // u32 offset = BIT_OFFSET(frame);
// 
    // frames[idx] &=  ~(0x1 << offset);
// }


static u32 frame_first_free() {
    for (u32 i = 0; i < BIT_INDEX(nframes); i++) {
        if(frames[i] != 0xFFFFFFFF) {
            for (u32 j = 0; j < 32; j++) {
                u32 tested_bit = 0x1 << j;
                if(!(frames[i] & tested_bit)) {
                    return i * 32 + j;
                }
            }
        }
    }
    return (u32)-1;
}


void frame_alloc(Page* page,bool is_kernel,bool is_writable) {
    if(page->frame) {
        printf("%x\n",page->frame);
        return;
    }

    u32 idx = frame_first_free();
    if(idx == (u32) -1) {
        PANIC("No Frame Available")
        return;
    }
    frame_set(idx * 0x1000);
    page->present = 1;
    page->rw = is_writable;
    page->user = is_kernel ? 0 : 1;
    page->frame = idx;

}

PageDir* page_dir;

Page* get_page(u32 addr,bool make,PageDir* dir) {
    addr /= 0x1000;
    u32 idx = addr/1024;


    if(dir->tables[idx] != 0) {
        return &dir->tables[idx]->pages[addr % 1024];
    } else if(make) {
        page_dir->tables[idx] = (PageTable*) kmalloc(sizeof(PageTable),1,0);
        memset((void*) &page_dir->tables[idx]->pages,0,1024);
        page_dir->tables_phy[idx] = ((u32) page_dir->tables[idx] | 0x3);
        return &dir->tables[idx]->pages[addr % 1024];
    }
    return 0;
}
void paging_init() {
    u32 mem_size = 0x1000000;
    nframes = mem_size / 0x1000;
    frames = (u32*) kmalloc(BIT_INDEX(nframes),0,0);
    memset(frames,0,BIT_INDEX(nframes));
    

    page_dir = (PageDir*) kmalloc(sizeof(PageDir),1,0);
    memset((void*)page_dir->tables,0,1024);

    unsigned long i = 0;
    while (i < free_mem_addr) {
        
        Page* page = get_page(i,1,page_dir);
        frame_alloc(page,0,1);

        i += 0x1000;
    }
    

    page_dir->table_addr = page_dir->tables_phy[0];
    paging_enable((u32) &page_dir->table_addr);    
}

void page_fault(Intrrupt_mdata regs) {
    u32 fault_addr;
    asm volatile("mov %%cr2 ,%0":"=r"(fault_addr));

    bool p = !(regs.err_code & 0x1);
    bool rw = !(regs.err_code & 0x2);
    bool user = !(regs.err_code & 0x4);
    bool reserved = !(regs.err_code & 0x8);
    // int id = !(regs.err_code & 0x10);

    printf("Page Fault \n");
    if(!p) printf("-    not present\n"); 
    if(rw) printf("-    readonly\n"); 
    if(user) printf("-    user-mode\n"); 
    if(reserved) printf("-    reserved\n");

    printf("at addr: %x\n",fault_addr);

    for (;;){}
}



// uses the addr of the first table
void paging_enable(u32 page_dir_addr) {
    asm volatile("mov %0, %%cr3":: "r"(page_dir_addr));
    u32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}
#endif