#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"
#include "mem.h"
#include "paging.h"

// #define SZ(x) (sizeof(x)/sizeof(x[0]))

// #define R_COM  0x63 // "common" bits

// #define R_W256 0x00
// #define R_W320 0x00
// #define R_W360 0x04
// #define R_W376 0x04
// #define R_W400 0x04

// #define R_H200 0x00
// #define R_H224 0x80
// #define R_H240 0x80
// #define R_H256 0x80
// #define R_H270 0x80
// #define R_H300 0x80
// #define R_H360 0x00
// #define R_H400 0x00
// #define R_H480 0x80
// #define R_H564 0x80
// #define R_H600 0x80


// static const u8 hor_regs [] = { 0x0,  0x1,  0x2,  0x3,  0x4,0x5,  0x13 };

// static const u8 width_256[] = { 0x5f, 0x3f, 0x40, 0x82, 0x4a, 0x9a, 0x20 };
// static const u8 width_320[] = { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0x28 };
// static const u8 width_360[] = { 0x6b, 0x59, 0x5a, 0x8e, 0x5e, 0x8a, 0x2d };
// static const u8 width_376[] = { 0x6e, 0x5d, 0x5e, 0x91, 0x62, 0x8f, 0x2f };
// static const u8 width_400[] = { 0x70, 0x63, 0x64, 0x92, 0x65, 0x82, 0x32 };

// static const u8 ver_regs  [] = { 0x6,  0x7,  0x9,  0x10, 0x11, 0x12, 0x15, 0x16 };

// static const u8 height_200[] = { 0xbf, 0x1f, 0x41, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
// static const u8 height_224[] = { 0x0b, 0x3e, 0x41, 0xda, 0x9c, 0xbf, 0xc7, 0x04 };
// static const u8 height_240[] = { 0x0d, 0x3e, 0x41, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
// static const u8 height_256[] = { 0x23, 0xb2, 0x61, 0x0a, 0xac, 0xff, 0x07, 0x1a };
// static const u8 height_270[] = { 0x30, 0xf0, 0x61, 0x20, 0xa9, 0x1b, 0x1f, 0x2f };
// static const u8 height_300[] = { 0x70, 0xf0, 0x61, 0x5b, 0x8c, 0x57, 0x58, 0x70 };
// static const u8 height_360[] = { 0xbf, 0x1f, 0x40, 0x88, 0x85, 0x67, 0x6d, 0xba };
// static const u8 height_400[] = { 0xbf, 0x1f, 0x40, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
// static const u8 height_480[] = { 0x0d, 0x3e, 0x40, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
// static const u8 height_564[] = { 0x62, 0xf0, 0x60, 0x37, 0x89, 0x33, 0x3c, 0x5c };
// static const u8 height_600[] = { 0x70, 0xf0, 0x60, 0x5b, 0x8c, 0x57, 0x58, 0x70 };

// // the chain4 parameter should be 1 for normal 13h-type mode, but //
// // only allows 320x200 256x200, 256x240 and 256x256 because you
// // can only access the first 64kb

// // if chain4 is 0, then plane mode is used (tweaked modes), and
// // you'll need to switch planes to access the whole screen but
// // that allows you using any resolution, up to 400x600

// int init_graph_vga(int width, int height,int chain4)
//   // returns 1=ok, 0=fail
// {
//    const u8 *w,*h;
//    u8 val;
//    int a;

//    switch(width) {
//       case 256: w=width_256; val=R_COM+R_W256; break;
//       case 320: w=width_320; val=R_COM+R_W320; break;
//       case 360: w=width_360; val=R_COM+R_W360; break;
//       case 376: w=width_376; val=R_COM+R_W376; break;
//       case 400: w=width_400; val=R_COM+R_W400; break;
//       default: return 0; // fail
//    }
//    switch(height) {
//       case 200: h=height_200; val|=R_H200; break;
//       case 224: h=height_224; val|=R_H224; break;
//       case 240: h=height_240; val|=R_H240; break;
//       case 256: h=height_256; val|=R_H256; break;
//       case 270: h=height_270; val|=R_H270; break;
//       case 300: h=height_300; val|=R_H300; break;
//       case 360: h=height_360; val|=R_H360; break;
//       case 400: h=height_400; val|=R_H400; break;
//       case 480: h=height_480; val|=R_H480; break;
//       case 564: h=height_564; val|=R_H564; break;
//       case 600: h=height_600; val|=R_H600; break;
//       default: return 0; // fail
//    }

//    // chain4 not available if mode takes over 64k

//    if(chain4 && (long)width*(long)height>65536L) return 0;

//    // here goes the actual modeswitch

//    port_byte_out(0x3c2,val);
//    port_word_out(0x3d4,0x0e11); // enable regs 0-7

//    for(a=0;a<SZ(hor_regs);++a)
//       port_word_out(0x3d4,(u16)((w[a]<<8)+hor_regs[a]));
//    for(a=0;a<SZ(ver_regs);++a)
//       port_word_out(0x3d4,(u16)((h[a]<<8)+ver_regs[a]));

//    port_word_out(0x3d4,0x0008); // vert.panning = 0

//    if(chain4) {
//       port_word_out(0x3d4,0x4014);
//       port_word_out(0x3d4,0xa317);
//       port_word_out(0x3c4,0x0e04);
//    } else {
//       port_word_out(0x3d4,0x0014);
//       port_word_out(0x3d4,0xe317);
//       port_word_out(0x3c4,0x0604);
//    }

//    port_word_out(0x3c4,0x0101);
//    port_word_out(0x3c4,0x0f02); // enable writing to all planes
//    port_word_out(0x3ce,0x4005); // 256color mode
//    port_word_out(0x3ce,0x0506); // graph mode & A000-AFFF

//    port_byte_in(0x3da);
//    port_byte_out(0x3c0,0x30); port_byte_out(0x3c0,0x41);
//    port_byte_out(0x3c0,0x33); port_byte_out(0x3c0,0x00);

//    for(a=0;a<16;a++) {    // ega pal
//       port_byte_out(0x3c0,(u8)a);
//       port_byte_out(0x3c0,(u8)a);
//    }
   
//    port_byte_out(0x3c0, 0x20); // enable video

//    return 1;

// }


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

void kernel_init() {
    init_screen_driver();
    isr_init();

    sti();
    init_timer(1);
    init_keyboard_driver();

  




    //  allocate memry after the screen vga stuff
    // free_mem_addr = 0xC0000;
    // paging_init();

}



static u32* frame_buffer = 0xA0000;
extern u32 tick;


void gfx_draw_pixel(u32 x,u32 y,u8 c) {
    x %= 320;
    u32 offset = ((x % 4) * 8);

    u64 color =  c | 0x00000000;
    color <<= offset;

    x /= 4;
    frame_buffer[y * 80 + x] &= ~(0x00000000FF << offset);
    frame_buffer[y * 80 + x] |= color;
}

void gfx_clear(u8 c) {
    for (u32 j = 0; j < 200; j++) {
        for (u32 i = 0; i < 320; i++) {
            frame_buffer[j * 320 + i] = c;
        }
    }
}
void gfx_draw_rect(u32 x,u32 y,u32 w, u32 h,u8 c) {
    for (u32 j = y; j < h + y; j++) {
        for (u32 i = x; i < w + x; i++) {
            gfx_draw_pixel(i, j,c);
        }
    }
}


void kernel_main() {
    kernel_init();
    clear_screen();

    


    unsigned char g_320x200x256[] =
    {
    /* general registers */
       0x63,
       /* sequencer */
       0x3,0x1,0xF,0,0xE,
       /* CRTC */
       0x5F,0x4F,0x50,0x82,0x24,0x80,0xBF,0x1F,0,0x41,
       0,0,0,0,0,0x31,0x9C,0x8E,0x8F,0x28,0x40,0x96,0x89,0xA3,0xFF,
       /* graphics */
       0,0,0,0,0,0x40,0x5,0xF,0xFF,
       /* attribute */
       0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF,0x41,0,0xF,0,0
    };

    write_regs(g_320x200x256);
    gfx_clear(0);

    u32 x = 50;
    while (1) {
        gfx_clear(0);
        gfx_draw_rect(x,0,28,50,0xF);
        x += 10;
        keybaord_get_char();
    }
    
}