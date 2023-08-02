#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"


void main() {
    init_screen_driver();
    clear_screen();

    isr_install();


    asm volatile("sti");
    init_timer(60);
    init_keyboard_driver();
}