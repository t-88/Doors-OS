#include "shared.h"
#include "screen.h"
#include "idt.h"





void main(void) {
    init_screen_driver();
    
    clear_screen();
    for (int i = 0; i < MAX_ROWS+1; i++) {
        kprint_char_at('0' + i,-1,-1,-1);
        kprint_char_at('0' + i,-1,-1,-1);
        kprint_char_at('0' + i,-1,-1,-1);
        kprint_char_at('0' + i,-1,-1,-1);
        kprint_char_at('0' + i,-1,-1,-1);
        kprint("\n",-1);
    }
        
    while (1){}
}