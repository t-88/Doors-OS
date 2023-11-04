#ifndef PAGING_H_
#define PAGING_H_

#include "shared.h"
#include "mem.h"
#include "isr.h"

typedef struct PageTable {
    u32 pages[1024];
} PageTable;

typedef struct PageDir {
    PageTable* tables[1024];
} PageDir;



void paging_enable(u32 addr);
void paging_init_identity_map();
void page_fault(Intrrupt_mdata regs);

extern PageDir* page_dir;

#endif

// #define PAGING_IMPLEMENTATION_C
#ifdef PAGING_IMPLEMENTATION_C
#include "shared.h"
#include "mem.h"
#include "isr.h"

PageDir* page_dir;

void paging_init_identity_map() {
    page_dir = (PageDir*) kmalloc(sizeof(PageDir),1,0);

    u64 addr = 0;
    PageTable* page_table;

    for (u32 i = 0; i < 1024; i++) {
        page_table = (PageTable*) kmalloc(sizeof(PageTable),1,0);
        for (u32 j = 0; j < 1024; j++) {
            page_table->pages[j] = addr | 3;
            addr += 4096; 
        }
        page_dir->tables[i] = ((u32)&page_table->pages[0]) | 3;
    }
    paging_enable(page_dir);
}


void paging_enable(u32 page_dir_addr) {
    // uses the addr of the first table
    asm volatile("mov %0, %%cr3":: "r"(page_dir_addr));
    u32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}


void page_fault(Intrrupt_mdata regs) {
    u32 fault_addr;
    asm volatile("mov %%cr2 ,%0":"=r"(fault_addr));

    bool p = !(regs.err_code & 0x1);
    bool rw = !(regs.err_code & 0x2);
    bool user = (regs.err_code & 0x4);
    bool reserved = !(regs.err_code & 0x8);
    // int id = !(regs.err_code & 0x10);

    printf("Page Fault at addr 0x%x\n",fault_addr);
    printf("-    operation %s\n",rw ? "read" : "write");
    printf("-    running in %s mode\n",user ? "user" : "kernel");
    if(p) printf("-    page is not present\n"); 
    if(reserved) printf("-    reserved bits were overwiten\n");

    STOP;
}
#endif