#include "shared.h"
#include "keyboard.h"
#include "print.h"

#ifndef TERMINAL_H_
#define TERMINAL_H_

typedef struct TerminalCmd {
    char cmd[10];
    void* user_data;
    void (*callback)(void*);
} TerminalCmd; 


static bool recved_cmd;
static u32 char_count ;


#define TERMINAL_BUFFER_MAX 25 
static char terminal_buffer[TERMINAL_BUFFER_MAX];

#define MAX_COMMANDS_COUNT 10
static u32 commands_count;
static TerminalCmd cmds[MAX_COMMANDS_COUNT];


void init_terminal();
void terminal_run();
static void terminal_handle_single_char(const char chr);
static void terminal_parse_cmd();
void terminal_add_cmd(char* cmd,void* callback, void* user_data);


void terminal_cmd_unknown();
void terminal_cmd_help();


#endif 

// #define TERMINAL_IMPLEMENETAION_C 
#ifdef TERMINAL_IMPLEMENETAION_C
void init_terminal() {
    recved_cmd = true;
    char_count =  2;
    commands_count = 0;


    terminal_add_cmd("help",terminal_cmd_help,0);
}

void terminal_run() {
    while (true) {
        // new line terminal '> '
        if(recved_cmd) {
            recved_cmd = false;
            printf("> ");
        }
        
        const char chr = keyboard_pop_from_buffer(); // read input
        terminal_handle_single_char(chr);
    }    
}


static void terminal_handle_single_char(char chr) {
    if(chr == 0) { return; }             // skip if there is none
    
    if(chr == '\n') {
        printf("\n");
        terminal_buffer[char_count - 2] = '\0';
        terminal_parse_cmd();

        recved_cmd = true;
        char_count = 2;
    } 
    else {
        if(chr == 8) {
            if(char_count > 2) {
                char_count--;
                
                printf("%c",chr);
                return;
            }
        } else {
            printf("%c",chr);
            terminal_buffer[char_count - 2] = chr;
            char_count++;
        }
    }
}


void terminal_cmd_help() {
    printf("cmds:\n");
    for(u32 i = 0; i < commands_count; i++) {
        printf("    - %s\n",cmds[i].cmd);
    }
    
}

void terminal_cmd_unknown() {
    printf("uknown command provided, to get command list type help\n");
}

static void terminal_parse_cmd() {
    for(u32 i = 0; i < commands_count; i++) {
        if(strcmp(terminal_buffer,cmds[i].cmd)) {
            cmds[i].callback(cmds[i].user_data);
            return;
        }
    }
    terminal_cmd_unknown();

}

void terminal_add_cmd(char* cmd_text,void* callback, void* user_data) {
    TerminalCmd cmd;

    u8 idx = 0;
    while(cmd_text[idx]) { cmd.cmd[idx] = cmd_text[idx]; idx++; }
    cmd.cmd[idx] = '\0';

    cmd.callback = callback;
    cmd.user_data = user_data;

    cmds[commands_count++] = cmd;
    
    if(commands_count == MAX_COMMANDS_COUNT) {
        PANIC("[Terminal Err] reached the MAX_COMMANDS_COUNT");
    } 
}
#endif