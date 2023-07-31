#ifndef SCREEN_H_
#define SCREEN_H_

#include "shared.h"
#include "port.h"

#define VIDEO_MEM 0xb8000 
#define MAX_ROWS 25 
#define MAX_COLS 80 

#define ATTR_WHITE_ON_BLACK 0x0F

uint8_t* video_mem;

    
void init_screen_driver();

uint32_t get_cursor_pos();
void set_cursor_pos(uint32_t);

void clear_screen();
void kprint_char_at(uint8_t chr,int attr,int x, int y);
void kprint_at(char* str,char attr,int x, int y);
void kprint(char* str,char attr);

void init_screen_driver() {
    video_mem = (uint8_t*) VIDEO_MEM;
}

uint32_t get_cursor_pos() {
    uint32_t cursor_pos;
    
    port_byte_out(0x3d4,14);
    cursor_pos =  port_byte_in(0x3d5);
    cursor_pos <<= 8;
    port_byte_out(0x3d4,15);
    cursor_pos |=  port_byte_in(0x3d5);

    return cursor_pos;
}
void set_cursor_pos(uint32_t cursor_pos) {
    port_byte_out(0x3d4,14);
    port_byte_out(0x3d5,cursor_pos >> 8);
    port_byte_out(0x3d4,15);
    port_byte_out(0x3d5,cursor_pos & 0xFF);
}


void clear_screen() {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video_mem[2 * i]  = ' ';
        video_mem[2 * i + 1]  = 0x0F;
    }

    set_cursor_pos(0);
}
void kprint_char_at(uint8_t chr,int attr,int x, int y) {
    if(attr < 0) {
        attr = ATTR_WHITE_ON_BLACK;
    }
    if(chr == '\n') {
        uint32_t cursor_pos = get_cursor_pos();
        set_cursor_pos((cursor_pos / MAX_COLS + 1) * MAX_COLS);
    } else {
        if(x == -1 || y == -1) {
            uint32_t cursor_pos = get_cursor_pos();
            video_mem[2 * cursor_pos] = chr;
            video_mem[2 * cursor_pos + 1] = attr;
            set_cursor_pos(cursor_pos + 1);
        } else {
            video_mem[2 * (x + y * MAX_COLS)] = chr;
            video_mem[2 * (x + y * MAX_COLS) + 1] = attr;
        }
    }
}
void kprint_at(char* str,char attr,int x, int y) {
    int index = 0;
    if(x == -1 | y == -1) {
        while (str[index]){
            kprint_char_at(str[index],attr,-1,-1);
            index++;
        }
    } else {
        while (str[index]){
            kprint_char_at(str[index],attr,x+index % MAX_COLS,y + index / MAX_COLS);
            index++;
        }
    }
}

void kprint(char* str,char attr) {
    kprint_at(str,attr,-1,-1);
}



#endif //SCREEN_H_

