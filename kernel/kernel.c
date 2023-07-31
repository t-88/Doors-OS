#include "shared.h"
#include "screen.h"




void main(void) {
    init_screen_driver();
    
    clear_screen();
    
    kprint("a\n",-1);
    kprint("b\n",-1);

    while (1){}
}