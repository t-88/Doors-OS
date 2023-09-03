#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "mem.h"
#include "paging.h"
#include "mouse.h"


#define KEYBOARD_DRIVER_IMPLEMENTATION
#include "keyboard.h"
#define TERMINAL_IMPLEMENETAION_C
#include "terminal.h"


int screen_width = 320;
int screen_height = 200;

void write_regs(u8* regs) {
    port_byte_out(0x3C2,*regs);
    regs++;
    for (u8 i = 0; i < 5; i++) {
        port_byte_out(0x3C4,i);
        port_byte_out(0x3C5,*regs);
        regs++;
    }

    port_byte_out(0x3D4, 0x03);
	port_byte_out(0x3D5, port_byte_in(0x3D5) | 0x80);
	port_byte_out(0x3D4, 0x11);
	port_byte_out(0x3D5, port_byte_in(0x3D5) & ~0x80);

	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
    for (u8 i = 0; i < 25; i++) {
        port_byte_out(0x3D4,i);
        port_byte_out(0x3D5,*regs);
        regs++;
    }

    for (u8 i = 0; i < 9; i++) {
        port_byte_out(0x3CE,i);
        port_byte_out(0x3CF,*regs);
        regs++;
    }

    for (u8 i = 0; i < 9; i++) {
        (void)port_byte_in(0x3DA);
        port_byte_out(0x3C0,i);
        port_byte_out(0x3C0,*regs);
        regs++;
    }
    (void)port_byte_in(0x3DA);
    port_byte_out(0x3C0,0x20);

}


u64* mem;

void kernel_init() {
    init_screen_driver();
    isr_init();

    sti();
    init_timer(1);
    init_keyboard_driver();

    free_mem_addr = 0x2000000;

    mem = kmalloc(4 * 200 * 80,0,0);


    // paging_init();

}


static u32* frame_buffer = 0xA0000;
extern u32 tick;


void swap_buffers() {
    for (u32 j = 0; j < 200; j++) {
        for (u32 i = 0; i < 80; i++) {
            frame_buffer[j * 80 + i] = mem[j * 80 + i];
        }
    }
    
}    


void gfx_draw_pixel(u32 x,u32 y,u8 c) {
    if(x >= screen_width || y >= screen_height) {
        return;
    }
    u32 offset = ((x % 4) * 8);

    u64 color =  c | 0x00000000;
    color <<= offset;

    x /= 4;
    mem[y * 80 + x] &= ~(0x00000000FF << offset);
    mem[y * 80 + x] |= color;
}

u64 power(u64 a,u64 b) {
    u64 tmp = 1;
    for (u64 i = 0; i < b; i++) {
        tmp = a * tmp;
    }
    return tmp;    
}

void gfx_clear(u8 c) {
    u64 color =  c;
    for (u8 i = 0; i < 4; i++) {
        color |= c << (i * 8);
    }
    
    
    for (u32 i = 0; i < 80 * 200; i++) {
        mem[i] =  0x01010101;
    }
}
void gfx_draw_rect(u32 x,u32 y,u32 w, u32 h,u8 c) {
    for (u32 j = y; j < h + y; j++) {
        for (u32 i = x; i < w + x; i++) {
            gfx_draw_pixel(i, j,c);
        }
    }
}
void mouse_write(u8 a_write) //unsigned char
{
  port_byte_out(0x64, 0xD4);
  port_byte_out(0x60, a_write);
}

void kernel_main() {
    kernel_init();
    clear_screen();


    
    terminal_init();
    terminal_run();

    
    // port_byte_out(0x64, 0xA8);
    // 
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
    // unsigned char g_320x200x256[] =
    // {
    // /* general registers */
    //    0x63,
    //    /* sequencer */
    //    0x3,0x1,0xF,0,0xE,
    //    /* CRTC */
    //    0x5F,0x4F,0x50,0x82,0x24,0x80,0xBF,0x1F,0,0x41,
    //    0,0,0,0,0,0x31,0x9C,0x8E,0x8F,0x28,0x40,0x96,0x89,0xA3,0xFF,
    //    /* graphics */
    //    0,0,0,0,0,0x40,0x5,0xF,0xFF,
    //    /* attribute */
    //    0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF,0x41,0,0xF,0,0
    // };

    

    // write_regs(g_320x200x256);
    // while (1) {
    //     gfx_clear(0xe);
    //     gfx_draw_rect(mouse_x,mouse_y,5,5,0xF);

    //     swap_buffers();
    // }
}