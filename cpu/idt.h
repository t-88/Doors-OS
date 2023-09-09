#include "shared.h"
#include "types.h"


#ifndef IDT_H_
#define IDT_H_

typedef struct idt_entry {
    u16 irs_base_low;
    u16 selector;
    u8 zero;
    u8 flags; 
    u16 irs_base_high;
}__attribute__((packed)) idt_entry;

typedef struct idt_ptr {
    u16 limit;
    u32 base;
}__attribute__((packed)) idt_ptr;


#define IDT_ENTRIES 256

extern idt_entry idt[IDT_ENTRIES];
extern idt_ptr idtp;

extern void idt_register_gate(int i, u32 handler);
extern void idt_setup();


#endif

// #define IDT_IMPLEMENTATION_C
#if defined(IDT_IMPLEMENTATION_C) || defined(IDT_STATIC_IMPLEMENTATION_C)

idt_entry idt[IDT_ENTRIES];
idt_ptr idtp;

void idt_register_gate(int i, u32 handler) {
    idt[i].irs_base_low = (u16)(handler & 0xFFFF);
    idt[i].irs_base_high = (u16)((handler >> 16) & 0xFFFF);
    idt[i].zero = 0;
    idt[i].selector = 0x08;
    idt[i].flags = 0x8E;
}
void idt_setup() {
    idtp.base = (u32) &idt;
    idtp.limit = 256 * sizeof(idt_entry) - 1;

    asm volatile ("lidtl (%0)" : : "r" (&idtp));
}
#endif