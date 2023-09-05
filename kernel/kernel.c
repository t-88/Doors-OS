#include "kernel.h"

u32 screen_width = 320;
u32 screen_height = 200;

void kernel_init() {
    init_screen_driver();
    isr_init();

    sti();
    init_timer(1);
    init_keyboard_driver();
    mmap_load_map();


    terminal_init();

    free_mem_addr = 0x2000000;
}



void kernel_main() {
    kernel_init();
    clear_screen();

    terminal_add_cmd("mmap",mmap_print_map,0);
    terminal_run();

    
}