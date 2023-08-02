#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "pic.h"

u32 tick = 0;


void timer_print_tick() {
    kprint("Tick: ",-1);
    char str[256];
    int_to_str(tick,str);
    kprint(str,-1);
    kprint("\n",-1);
}
static void timer_callback(intrrupt_mdata regs) {
    UN_USED(regs);
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