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

extern float mouse_x;
extern float mouse_y;

extern void mouse_init_driver();
extern void mouse_print_debug();



#define MOUSE_IMPLEMENTATION_C
#ifdef MOUSE_IMPLEMENTATION_C

bool mouse_down = false;
bool mouse_left = false;
bool mouse_right = false;
bool mouse_middle = false;

int dir_x = 0;
int dir_y = 0;
int mouse_x_delta = 0;
int mouse_y_delta = 0;

float mouse_x;
float mouse_y;

extern u32 screen_width;
extern u32 screen_height;

#endif

void mouse_print_debug() {
    if(!mouse_x_delta && !mouse_y_delta) { return; }

    printf("mouse:\n");
    printf("    x: %d\n",mouse_x);
    printf("    y: %d\n",mouse_y);

    if(mouse_x_delta) {
        printf("     delta x: %d \n",mouse_x_delta); 
        printf("     dir x:   %d \n",dir_x);         
    }
    if(mouse_y_delta) {
        printf("     delta y: %d \n",mouse_y_delta); 
        printf("     dir y:   %d \n",dir_y);         
    }
}

static void mouse_callback(Intrrupt_mdata reg) {
    
//     // y-overflow|x-overflow|y-sign|x-sign|1|m-btn|r-btn|l-btn
//     u8 packet1 =  port_byte_in(0x60);
//     io_wait();

//     mouse_left   = packet1 & 0x1; packet1 >>= 1;
//     mouse_right  = packet1 & 0x1; packet1 >>= 1;
//     mouse_middle = packet1 & 0x1; packet1 >>= 1;

//     mouse_down = mouse_left  || mouse_right || mouse_middle; 


//     packet1 >>= 1;
//     dir_x = (packet1 & 0x1) ? -1 : 1; packet1 >>= 1;
//     dir_y = (packet1 & 0x1) ? 1 : -1; packet1 >>= 1;
    
//     io_wait();

//     mouse_x_delta =  port_byte_in(0x60) | (dir_x == -1 ? 0xFFFFFF00 : 0);
//     if(packet1 & 0x1) {
//         mouse_x_delta = 0;
//     }
//     io_wait();
    
//     mouse_y_delta =  port_byte_in(0x60);
//     if(dir_y == -1) {
//         mouse_y_delta = -mouse_y_delta;
//     } else {
//         mouse_y_delta = -(mouse_y_delta | 0xFFFFFF00);
//     }
//     packet1 >>= 1;
//     if(packet1 & 0x1) {
//         mouse_y_delta = 0;
//     }

//     if(!mouse_x_delta && !mouse_x_delta) {
//         return;   
    // }


//     mouse_x += mouse_x_delta;
//     mouse_y += mouse_y_delta;


//     if(mouse_x  < 0) {
//         mouse_x = 0;
//     } else if (mouse_x   > screen_width) {
//         mouse_x = screen_width - 1;
//     }
//     if(mouse_y  < 0) {
//         mouse_y = 0;
//     } else if (mouse_y > screen_height) {
//         mouse_y = screen_height - 1;
//     }


    
//     mouse_print_debug();
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

    // mouse_x = screen_width / 2;
    // mouse_y = screen_height / 2;

    irq_register_handler(IRQ12,mouse_callback);
}





// TODO NOW:
// void mouse_init() {
    // port_byte_out(0x64, 0xA8);
    // port_byte_out(0x64,0x20);
    // u32 val = port_byte_in(0x60);
    // val |= 2;
    // val &= ~(1 << 5);
    // port_byte_out(0x64,0x60);
    // port_byte_out(0x60,val);
    // 
    // mouse_write(0xF6);
    // port_byte_in(0x60);
    // 
    // mouse_write(0xF4);
    // port_byte_in(0x60);
    // init_mouse_driver();
// }

// void mouse_write(u8 a_write) //unsigned char
// {
//   port_byte_out(0x64, 0xD4);
//   port_byte_out(0x60, a_write);
// }


#endif