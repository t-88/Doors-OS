#include "shared.h"
#include "isr.h"

#ifndef TIMER_H_
#define TIMER_H_

void init_timer(u32 freq);
void timer_print_tick();

#endif

// #define TIME_IMPLEMENTATION_C
#ifdef TIME_IMPLEMENTATION_C
u32 tick = 0;


void timer_print_tick() {
    printf("Tick: %d",tick);
}
static void timer_callback(Intrrupt_mdata regs) {
    tick++;
}

void init_timer(u32 freq) {
    irq_register_handler(IRQ0,timer_callback);

    u32 div = 1193180 / freq;
    u8 low = (u8) (div & 0xFF);
    u8 high = (u8) ((div >> 8) & 0xFF);

    port_byte_out(0x43,0x36);
    port_byte_out(0x40,low);
    port_byte_out(0x40,high);
}
#endif