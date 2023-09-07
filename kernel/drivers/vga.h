
#include "shared.h"
#include "port.h"

#ifndef VGA_H_
#define VGA_H_

//NOTE:
// vga is backward compatable with ega, i guess it explains the 64 colors instead of 256
// i have to do stuff in color palate option of vga to changes it
//TODO:
// add support for 256 colors

typedef enum DisplayMode {
    DisplayMode_320x200x256, // mode 13
} DisplayMode;



const u8 vga_mode_13[];
static u32* frame_buffer;

u64* mem;

void vga_init();
void vga_load_mode(DisplayMode mode);
static void vga_load_mode_general(u8* regs);


// Custom Graphics Functions
// for every mode there is a way to put pixles on screen

// Linear FrameBuffer
static void gfx_swap_buffers_320x200x256();
static void gfx_clear_screen_320x200x256(u8 c);
static void gfx_draw_rect_320x200x256(u32 x,u32 y,u32 w, u32 h,u8 c);
static void gfx_draw_pixel_320x200x256(u32 x,u32 y,u8 c);







// Graphics Variables
DisplayMode curr_mode;
u32 gfx_width;
u32 gfx_height;


// Graphics Function
void (*gfx_clear_screen)(u8 c);
void (*gfx_swap_buffers)();
void (*gfx_draw_pixel)(u32 x,u32 y,u8 c);
void (*gfx_draw_rect)(u32 x,u32 y,u32 w, u32 h,u8 c);


#endif

// #define VGA_IMPLEMENTATION
#ifdef VGA_IMPLEMENTATION
const u8 vga_mode_13[] = {
    // general regs
    0x63,
    // seq regs
    0x03,0x01,0x0F,0x00 ,0x0E,
    // CRTC regs
    0x5F,0x4F,0x50,0x82,0x24,0x80,0xBF,0x1F,0x00,0x41,
    0x00,0x00,0x00,0x00,0x00,0x31,0x9C,0x8E,0x8F,0x28,0x40,0x96,0x89,0xA3,0xFF,
    // GC regs
    0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0F,0xFF,
    // Attr regsz
    0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF,0x41,0,0xF,0,0,
};


void vga_init() {
    frame_buffer = (u32*) 0xA0000;
    mem = kmalloc(4 * 200 * 80,0,0);
}
void vga_load_mode(DisplayMode mode) {
    curr_mode = mode;
    switch (mode)
    {
        case DisplayMode_320x200x256:
            gfx_width = 320; gfx_height = 200;
            gfx_clear_screen = gfx_clear_screen_320x200x256;
            gfx_swap_buffers = gfx_swap_buffers_320x200x256;
            gfx_draw_pixel = gfx_draw_pixel_320x200x256;
            gfx_draw_rect = gfx_draw_rect_320x200x256;

            vga_load_mode_general((u8*) vga_mode_13);
        break;
        default:
            Unreachable("[Error] vga_load_mode unknown mode");
        break;
    }
}

static void vga_load_mode_general(u8* regs) {
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
    port_byte_in(0x3DA);
    port_byte_out(0x3C0,0x20);
}


// Custom Graphics Functions
// Linear FrameBuffer
// 320x200x256- mode13
void gfx_swap_buffers_320x200x256() {
    for (u32 j = 0; j < 200; j++) {
        for (u32 i = 0; i < 80; i++) {
            frame_buffer[j * 80 + i] = mem[j * 80 + i];
        }
    }
}    


static void gfx_clear_screen_320x200x256(u8 c) {
    u64 color = c;
    color =  color | (color << 8) | (color << 16) | (color << 24);
        
    // for (u8 i = 0; i < 4; i++) {
        // color |= c << (i * 8);
    // }
    
    for (u32 i = 0; i < 80 * 200; i++) {
        mem[i] =  color;
    }
}
static void gfx_draw_pixel_320x200x256(u32 x,u32 y,u8 c) {
    if(x >= gfx_width || y >= gfx_height) {
        return;
    }

    u32 offset = ((x % 4) * 8);
    u64 color =  c | 0x00000000;
    color <<= offset;
    x /= 4;
    mem[y * 80 + x] &= ~(0x00000000FF << offset);
    mem[y * 80 + x] |= color;
}
static void gfx_draw_rect_320x200x256(u32 x,u32 y,u32 w, u32 h,u8 c) {
    for (u32 j = y; j < h + y; j++) {
        for (u32 i = x; i < w + x; i++) {
            gfx_draw_pixel_320x200x256(i, j,c);
        }
    }
}
#endif