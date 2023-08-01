#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"



void main() {
    init_screen_driver();
    // clear_screen();

    kprint("\n(1)\n",-1);   
    
    isr_install();
    __asm__ __volatile__("int $0");
    kprint("(5)\n",-1);   
}