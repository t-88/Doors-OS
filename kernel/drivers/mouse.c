#include "mouse.h"
#include "shared.h"
#include "isr.h"
#include "screen.h"


bool mouse_down = false;
bool mouse_left = false;
bool mouse_right = false;
bool mouse_middle = false;

int dir_x = 0;
int dir_y = 0;
int mouse_x_delta = 0;
int mouse_y_delta = 0;

int mouse_x;
int mouse_y;

extern u32 screen_width;
extern u32 screen_height;

void mouse_print_debug() {
    PRINTLN("mouse:");
    PRINT("     x: "); PRINT_INT(mouse_x); PRINTLN();
    PRINT("     y: "); PRINT_INT(mouse_y); PRINTLN();

#if 1
    if(mouse_x_delta) {
        PRINT("     delta x: "); PRINT_INT(mouse_x_delta); PRINTLN();
        PRINT("     dir x: "); PRINT_INT(dir_x); PRINTLN();
    }
    if(mouse_y_delta) {
        PRINT("     delta y: "); PRINT_INT(mouse_y_delta); PRINTLN();
        PRINT("     dir y: "); PRINT_INT(dir_y); PRINTLN();
    }
#endif
}

static void mouse_callback(Intrrupt_mdata reg) {
    UN_USED(reg);
    // PRINTLN("-------------------*mouse works*------------------");
    
    // y-overflow|x-overflow|y-sign|x-sign|1|m-btn|r-btn|l-btn
    u8 packet1 =  port_byte_in(0x60);
    io_wait();

    mouse_left   = packet1 & 0x1; packet1 >>= 1;
    mouse_right  = packet1 & 0x1; packet1 >>= 1;
    mouse_middle = packet1 & 0x1; packet1 >>= 1;

    mouse_down = mouse_left  || mouse_right || mouse_middle; 


    packet1 >>= 1;
    dir_x = (packet1 & 0x1) ? -1 : 1; packet1 >>= 1;
    dir_y = (packet1 & 0x1) ? 1 : -1; packet1 >>= 1;
    

    mouse_x_delta =  port_byte_in(0x60) | (dir_x == -1 ? 0xFFFFFF00 : 0);
    if(packet1 & 0x1) {
        mouse_x_delta = 0;
    }
    
    mouse_y_delta =  port_byte_in(0x60);
    if(dir_y == -1) {
        mouse_y_delta *= -1;
    } else {
        mouse_y_delta = -(mouse_y_delta | 0xFFFFFF00);
    }
    packet1 >>= 1;
    if(packet1 & 0x1) {
        mouse_y_delta = 0;
    }

    mouse_x += mouse_x_delta;
    mouse_y += mouse_y_delta;

    if(mouse_x  < 0) {
        mouse_x = 0;
    } else if (mouse_x   > screen_width) {
        mouse_x = screen_width - 1;
    }
    if(mouse_y  < 0) {
        mouse_y = 0;
    } else if (mouse_y > screen_height) {
        mouse_y = screen_height - 1;
    }

    
    // mouse_print_debug();
}

void init_mouse_driver() {
    mouse_down = false;
    mouse_left = false;
    mouse_right = false;
    mouse_middle = false;


    dir_x = 0;
    dir_y = 0;
    mouse_x_delta = 0;
    mouse_y_delta = 0;

    mouse_x = screen_width / 2;
    mouse_y = screen_height / 2;

    irq_register_handler(IRQ12,mouse_callback);
}