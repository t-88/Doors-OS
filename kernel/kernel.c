#include "kernel.h"



void kernel_init() {
    free_mem_addr = 0x100000;

    init_screen_driver();
    isr_init();
    mmap_load_map();
    terminal_init();
    vga_init();



    sti();
    init_timer(1);
    init_keyboard_driver();


    // paging_init();
}


void vga_graphics() {
    vga_load_mode(DisplayMode_320x200x256);
    gfx_clear_screen(0);
 
    u8 row_lenght = 20;
    for (u8 i = 0; i < 64; i++) {
        gfx_draw_rect((i % row_lenght) * 10, (i / row_lenght) * 10,10,10,i);
    }
    
    gfx_swap_buffers();
}


void kernel_main() {
    kernel_init();
    clear_screen();



    Heap_t* heap = heap_create(KHEAP_START,KHEAP_START + KHEAP_SIZE,KHEAP_SIZE);
    

    u32 addrs[7];
    addrs[0] = (u32) heap_alloc(heap,4);
    addrs[1] = (u32) heap_alloc(heap,4);
    addrs[2] = (u32) heap_alloc(heap,4);


    printf("0x%x\n",addrs[0]);
    printf("0x%x\n",addrs[1]);
    printf("0x%x\n",addrs[2]);

    heap_free(heap,addrs[0]);
    addrs[3] = (u32) heap_alloc(heap,4);
    printf("0x%x\n",addrs[3]);



    terminal_add_cmd("mmap",mmap_print_map,0);
    terminal_add_cmd("vga",vga_graphics,0);
    terminal_run();

    STOP;
}