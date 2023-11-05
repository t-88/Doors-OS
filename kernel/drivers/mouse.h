#include "shared.h"
#include "print.h"
#include "isr.h"


#ifndef MOUSE_H_
#define MOUSE_H_

extern bool mouse_down;
extern bool mouse_left;
extern bool mouse_right;
extern bool mouse_middle;

extern int dir_x;
extern int dir_y;
extern int mouse_x_delta;
extern int mouse_y_delta;

extern int mouse_x;
extern int mouse_y;

extern void mouse_init_driver();
extern void mouse_init_sequence();

extern void mouse_print_debug();

#endif


#define MOUSE_IMPLEMENTATION_C
#ifdef MOUSE_IMPLEMENTATION_C

#include "macros.h"
#include "math.h"
#include "vga.h"

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

extern u32 gfx_width , gfx_height;
int mouse_cycle = 0;
u8 mouse_packet[3];
 

const u8 mouse_icon[] = {
    1,1,1,1,1,0, 
    1,1,1,1,0,0, 
    1,1,1,1,0,0,
    1,1,1,1,1,0,
    1,0,0,1,1,1,
    0,0,0,0,1,1,
};  

#define MOUSE_ICON_COLOR 5


void mouse_draw_cursor() {
    for (u32 y = 0; y < 6; y++) {
        for (u32 x = 0; x < 6; x++) {
            if(mouse_icon[x + y *   6]) {
                gfx_draw_pixel(mouse_x + x,mouse_y + y,MOUSE_ICON_COLOR);
            }
        }
    }
}

static void mouse_callback(Intrrupt_mdata reg) {
    // save packet in packet list
    mouse_packet[mouse_cycle] = port_byte_in(0x60);

    // skip overflow packet
    if(mouse_cycle == 0) {
        bool x_o  = (mouse_packet[0] >> 6) & 0x1;  
        bool y_o  = (mouse_packet[0] >> 7) & 0x1;

        // if overflow
        if(x_o || y_o) return;
    } 
    
    // collect next packet
    mouse_cycle += 1;

    if(mouse_cycle == 3) {
        bool l_btn = (mouse_packet[0] >> 0) & 0x1;
        bool r_btn = (mouse_packet[0] >> 1) & 0x1;
        bool m_btn = (mouse_packet[0] >> 2) & 0x1;

        bool x_sign = (mouse_packet[0] >> 4) & 0x1;
        bool y_sign = (mouse_packet[0] >> 5) & 0x1;

        int rel_x = mouse_packet[1];
        int rel_y = mouse_packet[2];

        if(x_sign) rel_x |= 0xFFFFFF00;
        if(y_sign) rel_y |= 0xFFFFFF00;

        // boundy checks
        mouse_x = max_i(0,min_i(mouse_x + rel_x,gfx_width));
        mouse_y = max_i(0,min_i(mouse_y - rel_y,gfx_height));
    }

    mouse_cycle = mouse_cycle % 3;

   
}
void mouse_init_sequence() {
    cli();

    port_byte_out(0x64, 0xA8);
    port_byte_out(0x64,0x20);

    u32 val = port_byte_in(0x60);
    val |= 2;
    port_byte_out(0x64,0x60);
    port_byte_out(0x60,val);
    
    mouse_write(0xF6);
    port_byte_in(0x60);

    mouse_write(0xF4);
    port_byte_in(0x60);


    sti();
}
void mouse_write(u8 a_write) {
  port_byte_out(0x64, 0xD4);
  port_byte_out(0x60, a_write);
}
void init_mouse_driver() {
    mouse_init_sequence();

    mouse_down = false;
    mouse_left = false;
    mouse_right = false;
    mouse_middle = false;

    dir_x = 0;
    dir_y = 0;
    mouse_x_delta = 0;
    mouse_y_delta = 0;

    mouse_x = gfx_width / 2;
    mouse_y = gfx_height / 2;

    mouse_cycle = 0;
    for (int i = 0; i < 3; i++) {
        mouse_packet[i] = 0;
    }
    

    irq_register_handler(IRQ12,mouse_callback);
}





#endif