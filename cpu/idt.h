#pragma once
#include "shared.h"
#include "types.h"


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


extern void set_idt_gate(int i, u32 handler);
extern void set_idt();