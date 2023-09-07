#include "shared.h"
#include "port.h"
#include "mem.h"



#ifndef SCREEN_H_
#define SCREEN_H_

#define VIDEO_MEM 0xb8000 
#define MAX_ROWS 25 
#define MAX_COLS 80 
#define ATTR_WHITE_ON_BLACK 0x0F

uint8_t* video_mem;
    
extern void init_screen_driver();
static uint32_t get_cursor_pos();
static void set_cursor_pos(uint32_t);
static void clear_screen();
static void scroll_screen();


extern void kprint_char_at(char* chr,int attr,int x, int y);
extern void kprint_str_at(char* str,char attr,int x, int y);
extern void kprintc(char* str);
extern void kprints(char* str);

#endif

// #define SCREEN_IMPLEMENTATION_C
#ifdef SCREEN_IMPLEMENTATION_C

uint8_t* video_mem;

void init_screen_driver() {
    video_mem = (uint8_t*) VIDEO_MEM;
}

static void set_cursor_pos(uint32_t cursor_pos) {
    port_byte_out(0x3d4,14);
    port_byte_out(0x3d5,cursor_pos >> 8);
    port_byte_out(0x3d4,15);
    port_byte_out(0x3d5,cursor_pos & 0xFF);
}
static void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video_mem[2 * i]  = ' ';
        video_mem[2 * i + 1]  = 0x0F;
    }

    set_cursor_pos(0);
}
static void scroll_screen() {
    for (int y = 0; y < MAX_ROWS; y++) {
        for (int x = 0; x < MAX_COLS; x++) {
            kmemcpy(
                      video_mem + 2 * (x + (y +1) * MAX_COLS),
                      video_mem + 2 * (x + y * MAX_COLS),
                      2
                    );
        }
    }
}
static uint32_t get_cursor_pos() {
    uint32_t cursor_pos;
    
    port_byte_out(0x3d4,14);
    cursor_pos =  port_byte_in(0x3d5);
    cursor_pos <<= 8;
    port_byte_out(0x3d4,15);
    cursor_pos |=  port_byte_in(0x3d5);

    return cursor_pos;
}


void kprint_char_at(char* chr,int attr,int x, int y) {
    if(attr < 0) {
        attr = ATTR_WHITE_ON_BLACK;
    }
    if(chr == '\n') {
        uint32_t cursor_pos = get_cursor_pos();
        if(cursor_pos / MAX_COLS >= MAX_ROWS - 1 ) {
            scroll_screen();
            cursor_pos -= MAX_COLS;
        }
        set_cursor_pos((cursor_pos / MAX_COLS + 1) * MAX_COLS);
    }
    else if(chr == 8) {
        // backspace
        uint32_t cursor_pos = get_cursor_pos();
        if(cursor_pos == 0) {
            return;
        }

        cursor_pos -= 1;
        set_cursor_pos(cursor_pos);
        video_mem[2 * cursor_pos] = ' ';
        video_mem[2 * cursor_pos + 1] = attr;
    } else {
        if(x == -1 || y == -1) {
            uint32_t cursor_pos = get_cursor_pos();

            if(cursor_pos >= MAX_ROWS * MAX_COLS) {
                scroll_screen();
                cursor_pos -= MAX_COLS;
            }

            video_mem[2 * cursor_pos] = chr;
            video_mem[2 * cursor_pos + 1] = attr;
            set_cursor_pos(cursor_pos + 1);
        } else {
            video_mem[2 * (x + y * MAX_COLS)] = chr;
            video_mem[2 * (x + y * MAX_COLS) + 1] = attr;
        }
    }
}
void kprint_str_at(char* str,char attr,int x, int y) {
    int index = 0;
    if(x == -1 || y == -1) {
        while (str[index]){
            kprint_char_at(str[index],attr,-1,-1);
            index++;
        }
    } 
    else {
        while (str[index]){
            kprint_char_at(str[index],attr,x+index % MAX_COLS,y + index / MAX_COLS);
            index++;
        }
    }
}
void kprintc(char* str) {
    kprint_char_at(str,-1,-1,-1);
}
void kprints(char* str) {
    kprint_str_at(str,-1,-1,-1);
}


#endif