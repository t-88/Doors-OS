#include "kernel.h"


void draw_graphics();

void kernel_init() {
    free_mem_addr = 0x100000;


    init_isr();
    init_identity_paging_map();
    init_terminal();
    init_vga();

    mmap_load_map();

    sti();
    init_timer(1);
    init_screen_driver();
    init_keyboard_driver();
    init_mouse_driver();
}





void kernel_main() {
    kernel_init();
    clear_screen();


    gfx_width = 320;
    gfx_height = 200;


    terminal_add_cmd("mmap",mmap_print_map,0);
    terminal_add_cmd("vga",draw_graphics,0);
    terminal_run();


    STOP;
}

void init_graphics() {
    vga_load_mode(DisplayMode_320x200x256);
}
void update_graphics() {
    while (last_char != keyboard_char_to_scanecode(' ')) {
        gfx_clear_screen(0);

        mouse_draw_cursor();
        gfx_draw_rect_outline(0,0,gfx_width - 1,gfx_height - 1,1);


        gfx_swap_buffers();
    }
}
void draw_graphics() {
    init_graphics();
    update_graphics();
}
