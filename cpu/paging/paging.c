#include "paging.h"
#include "isr.h"
#include "mem.h"




u32* frames;
u32 nframes;


static void frame_set(u32 frame_addr) {
    u32 frame = frame_addr / 0x1000;
    u32 idx = BIT_INDEX(frame);
    u32 offset = BIT_OFFSET(frame);

    frames[idx] |=  (0x1 << offset);
}

static void frame_clear(u32 frame_addr) {
    u32 frame = frame_addr / 0x1000;
    u32 idx = BIT_INDEX(frame);
    u32 offset = BIT_OFFSET(frame);

    frames[idx] &=  ~(0x1 << offset);
}


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
        PRINT_HEX(page->frame)
        PRINTLN("");
        STOP;
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
        page_dir->tables[idx] = kmalloc(sizeof(PageTable),1,0);
        memset(&page_dir->tables[idx]->pages,0,1024);
        page_dir->tables_phy[idx] = ((u32) page_dir->tables[idx] | 0x3);
        return &dir->tables[idx]->pages[addr % 1024];
    }
    return 0;
}
void paging_init() {
    u32 mem_size = 0x1000000;
    nframes = mem_size / 0x1000;
    frames = kmalloc(BIT_INDEX(nframes),0,0);
    memset(frames,0,BIT_INDEX(nframes));
    

    page_dir = kmalloc(sizeof(PageDir),1,0);
    memset(page_dir->tables,0,1024);

    unsigned long i = 0;
    while (i < free_mem_addr) {
        
        Page* page = get_page(i,1,page_dir);
        frame_alloc(page,0,1);

        i += 0x1000;
    }
    

    page_dir->table_addr = page_dir->tables_phy[0];
    paging_enable(&page_dir->table_addr);    
}

void page_fault(Intrrupt_mdata regs) {
    UN_USED(regs);
    u32 fault_addr;
    asm volatile("mov %%cr2 ,%0":"=r"(fault_addr));

    bool p = !(regs.err_code & 0x1);
    bool rw = !(regs.err_code & 0x2);
    bool user = !(regs.err_code & 0x4);
    bool reserved = !(regs.err_code & 0x8);
    int id = !(regs.err_code & 0x10);
    UN_USED(id);

    kprint("Page Fault \n",-1);
    if(!p) kprint("-    not present\n",-1); 
    if(rw) kprint("-    readonly\n",-1); 
    if(user) kprint("-    user-mode\n",-1); 
    if(reserved) kprint("-    reserved\n",-1);


    kprint("at addr: ",-1);
    PRINT_HEX(fault_addr);
    kprint("\n",-1);

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