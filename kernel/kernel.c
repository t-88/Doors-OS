#include "shared.h"
#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "keyboard.h"

#include "mem.h"

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

    u32* addr = (u32*) kmalloc(10);
    *addr = 20000;

    char str[10];
    int_to_str(*addr,str);
    kprint("value: ",-1);
    kprint(str,-1);
    kprint("\n",-1);
}