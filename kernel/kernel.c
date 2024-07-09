#include "kernel.h"
#include "graphics.h"


bool app_menu_open = false; 

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

    app_menu_open = false;

    terminal_add_cmd("mmap",mmap_print_map,0);
    terminal_add_cmd("vga",draw_graphics,0);
    terminal_run();

    STOP;
}





window_t window;


void init_graphics() {
    window_init(&window,100,100,50,50);
    vga_load_mode(DisplayMode_320x200x256);
}



void update_graphics() {
    while(true) {
        gfx_draw_rect_outline(0,0,gfx_width - 1,gfx_height - 1,COLOR_BLUE);
        gfx_draw_rect_outline(0,gfx_height - 20,gfx_width - 1,19,COLOR_BLUE);
        if(button(1,gfx_height - 19,19,18,COLOR_CYAN)) {
            app_menu_open = !app_menu_open;
        }
        if(app_menu_open) {
            gfx_draw_rect(1,gfx_height - 121,80,100,6);
        }
    

        window_drag(&window);

        window_draw(window);
        mouse_draw_cursor();


        char text1[] = "Wellcome to my os user interface"; 
        gfx_draw_text(10,10,text1,32,COLOR_WHITE);
        char text2[] = "Ring 0 user interface XD"; 
        gfx_draw_text(10,20,text2,24,COLOR_WHITE);

        gfx_swap_buffers();
        gfx_clear_screen(COLOR_BLACK);
    }
}
void draw_graphics() {
    init_graphics();
    update_graphics();
}
