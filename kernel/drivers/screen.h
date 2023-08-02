#pragma once

#include "shared.h"
#include "port.h"
#include "mem.h"


#define VIDEO_MEM 0xb8000 
#define MAX_ROWS 25 
#define MAX_COLS 80 
#define ATTR_WHITE_ON_BLACK 0x0F

extern uint8_t* video_mem;

    
extern void init_screen_driver();
extern uint32_t get_cursor_pos();
extern void set_cursor_pos(uint32_t);
extern void clear_screen();
extern void kprint_char_at(uint8_t chr,int attr,int x, int y);
extern void kprint_at(char* str,char attr,int x, int y);
extern void kprint(char* str,char attr);
extern void scroll_screen();
