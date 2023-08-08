#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "mem.h"
#include "paging.h"



void kernel_init() {
    init_screen_driver();
    isr_init();

    sti();
    init_timer(1);
    init_keyboard_driver();



    //  allocate memry after the screen vga stuff
    free_mem_addr = 0xb8000 + 0x2000;
    paging_init();

}






void kernel_main() {
    kernel_init();
    clear_screen();

    


    u32 *ptr = (u32*)0xA0000000;
    u32 do_page_fault = *ptr;

    kprint("end of paging_init\n",-1);
}