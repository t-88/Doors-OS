#include "idt.h"
#include "screen.h"


idt_entry idt[IDT_ENTRIES];
idt_ptr idtp;

void set_idt_gate(int i, u32 handler) {
    idt[i].irs_base_low = (u16)(handler & 0xFFFF);
    idt[i].irs_base_high = (u16)((handler >> 16) & 0xFFFF);
    idt[i].zero = 0;
    idt[i].selector = 0x08;
    idt[i].flags = 0x8E;


}
void set_idt() {
    idtp.base = (u32) &idt;
    idtp.limit = 256 * sizeof(idt_entry) - 1;

    asm volatile ("lidtl (%0)" : : "r" (&idtp));
}