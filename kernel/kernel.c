#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"


void main() {
    init_screen_driver();
    clear_screen();

    isr_install();


    __asm__ __volatile__("sti");
    init_timer(60);
}