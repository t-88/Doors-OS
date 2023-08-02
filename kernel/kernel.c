#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"

void kernel_init() {
    init_screen_driver();
    isr_init();

    sti();
    init_timer(60);
    init_keyboard_driver();
}

void kernel_main() {
    kernel_init();
    clear_screen();

}