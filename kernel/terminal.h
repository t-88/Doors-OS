#pragma once

#include "shared.h"
#include "screen.h"
#include "keyboard.h"

static bool recved_cmd;
static u32 char_count ;

#define TERMINAL_BUFFER_MAX 25 
char terminal_buffer[TERMINAL_BUFFER_MAX];

void terminal_init();
void terminal_run();
void terminal_handle_single_char(const char chr);
void terminal_parse_cmd();

void terminal_cmd_unknown();

#define TERMINAL_IMPLEMENETAION_C 
#ifdef TERMINAL_IMPLEMENETAION_C
void terminal_init() {
    recved_cmd = true;
    char_count =  2;
}

void terminal_run() {
    while (true) {
        // new line terminal '> '
        if(recved_cmd) {
            recved_cmd = false;
            PRINT("> ");
        }
        
        const char chr = keyboard_pop_from_buffer(); // read input
        terminal_handle_single_char(chr);
    }    
}


void terminal_handle_single_char(char chr) {
    if(chr == 0) { return; }             // skip if there is none
    
    if(chr == '\n') {
        PRINTLN();
        terminal_buffer[char_count - 2] = '\0';
        terminal_parse_cmd();

        recved_cmd = true;
        char_count = 2;
    } 
    else {
        if(chr == 8) {
            if(char_count > 2) {
                char_count--;
                
                kprint_char((char*)chr,-1);
                return;
            }
        } else {
            kprint_char((char*)chr,-1);
            terminal_buffer[char_count - 2] = chr;
            char_count++;
        }
    }
}


void terminal_cmd_help() {
    PRINTLN("help:");
    PRINTLN("   ur mom: it gives you what you expect :)");
}

void terminal_cmd_unknown() {
    PRINTLN("uknown command provided, to get command list type help");
}

void terminal_parse_cmd() {
    if(strcmp(terminal_buffer,"ur mom")) {
        PRINTLN("ur mom too, right back at u");
    } else if(strcmp(terminal_buffer,"help")) {
        terminal_cmd_help();
    }
    else {
        terminal_cmd_unknown();
    }
}
#endif