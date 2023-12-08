#include "kernel.h"


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

void init_graphics() {
    vga_load_mode(DisplayMode_320x200x256);
}

bool ui_button(x,y,w,h,c) {
    static bool mouse_left_pressed = false;
    gfx_draw_rect(x,y,w,h,c);

    if(mouse_left_pressed && !mouse_left) {
        mouse_left_pressed = false;
        return false;
    }
    if(!mouse_left_pressed && mouse_left && AABB(x,y,w,h,mouse_x,mouse_y,2,2)) {
        mouse_left_pressed = true;
        return true;

    }

    return false;
}


bool ui_window(x,y,w,h,c) {
    static int prev_x = 0;
    static int prev_y = 0;
    static bool is_mouse_left_down = false;
    static int x_offset = 0;
    static int y_offset = 0;
    static int moved_x;
    static int moved_y;

    

    static bool is_inited = false;
    if(!is_inited) {
        moved_x = x;
        moved_y = y;
        is_inited = true;
    }


    static bool init_state = false;


    if(is_mouse_left_down && !mouse_left) {
        is_mouse_left_down = false;
        return;
    }

    if(!is_mouse_left_down && mouse_left && AABB(moved_x,moved_y,w,h,mouse_x,mouse_y,2,2)) {
        is_mouse_left_down = true;
        x_offset = mouse_x - moved_x;
        y_offset = mouse_y - moved_y ;
    }

    gfx_draw_rect(moved_x,moved_y,w,h,c);
    if(is_mouse_left_down && AABB(moved_x,moved_y,w,h,mouse_x,mouse_y,2,2)) {
        moved_x = mouse_x - x_offset;
        moved_y = mouse_y - y_offset;
        gfx_draw_rect(moved_x,moved_y,w,h,2);
    }
}

void update_graphics() {
    while (last_char != keyboard_char_to_scanecode(' ')) {
        gfx_clear_screen(0);

        gfx_draw_rect_outline(0,0,gfx_width - 1,gfx_height - 1,1);
        gfx_draw_rect_outline(0,gfx_height - 20,gfx_width - 1,20,1);
        if(ui_button(1,gfx_height - 19,19,18,2)) {
            app_menu_open = !app_menu_open;
        }
        if(app_menu_open) {
            gfx_draw_rect(1,gfx_height - 121,80,100,6);
        }
    

        ui_window(100,100,50,50,9);
        mouse_draw_cursor();

        gfx_swap_buffers();
    }
}
void draw_graphics() {
    init_graphics();
    update_graphics();
}
