#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"



void main() {
    init_screen_driver();
    clear_screen();

    isr_install();

    __asm__ __volatile__("int $0x0");
    __asm__ __volatile__("int $0x1");
    __asm__ __volatile__("int $0x3");
}