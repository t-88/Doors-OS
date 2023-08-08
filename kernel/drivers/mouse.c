#include "mouse.h"
#include "shared.h"
#include "isr.h"
#include "screen.h"
static void mouse_callback(Intrrupt_mdata reg) {
    UN_USED(reg );
    kprint("mouse works\n",-1);
    for(;;){}
}

void init_mouse_driver() {
    irq_register_handler(IRQ2,mouse_callback);
}