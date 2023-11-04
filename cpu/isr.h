#ifndef ISR_H_
#define ISR_H_

#include "shared.h"
#include "print.h"
#include "pic.h"
#include "idt.h"


/* ISRs reserved for CPU exceptions */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();



#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* Struct which aggregates many registers */
typedef struct Intrrupt_mdata {
   u32 ds; /* Data segment selector */
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   u32 int_no, err_code; /* Interrupt number and error code (if applicable) */
   u32 eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} __attribute__((packed)) Intrrupt_mdata;

extern void isr_init();
extern void isr_handler(Intrrupt_mdata r);

typedef void (*isr_t)(Intrrupt_mdata);
extern void irq_register_handler(u8 n, isr_t handler);
extern void irq_handler(Intrrupt_mdata r);


#endif

// #define ISR_IMPLEMENTATION_C
#ifdef ISR_IMPLEMENTATION_C
#include "shared.h"
#include "print.h"
#define PIC_STATIC_IMPLEMENTATION_C
#include "pic.h"
#define IDT_STATIC_IMPLEMENTATION_C
#include "idt.h"


isr_t interrupt_handlers[256];

void isr_init() {
    idt_register_gate(0,  (u32)isr0);
    idt_register_gate(1,  (u32)isr1);
    idt_register_gate(2,  (u32)isr2);
    idt_register_gate(3,  (u32)isr3);
    idt_register_gate(4,  (u32)isr4);
    idt_register_gate(5,  (u32)isr5);
    idt_register_gate(6,  (u32)isr6);
    idt_register_gate(7,  (u32)isr7);
    idt_register_gate(8,  (u32)isr8);
    idt_register_gate(9,  (u32)isr9);
    idt_register_gate(10, (u32)isr10);
    idt_register_gate(11, (u32)isr11);
    idt_register_gate(12, (u32)isr12);
    idt_register_gate(13, (u32)isr13);
    idt_register_gate(14, (u32)isr14);
    idt_register_gate(15, (u32)isr15);
    idt_register_gate(16, (u32)isr16);
    idt_register_gate(17, (u32)isr17);
    idt_register_gate(18, (u32)isr18);
    idt_register_gate(19, (u32)isr19);
    idt_register_gate(20, (u32)isr20);
    idt_register_gate(21, (u32)isr21);
    idt_register_gate(22, (u32)isr22);
    idt_register_gate(23, (u32)isr23);
    idt_register_gate(24, (u32)isr24);
    idt_register_gate(25, (u32)isr25);
    idt_register_gate(26, (u32)isr26);
    idt_register_gate(27, (u32)isr27);
    idt_register_gate(28, (u32)isr28);
    idt_register_gate(29, (u32)isr29);
    idt_register_gate(30, (u32)isr30);
    idt_register_gate(31, (u32)isr31);

    PIC_remap(PIC1_OFFSET,PIC2_OFFSET);
    idt_register_gate(32, (u32)irq0);
    idt_register_gate(33, (u32)irq1);
    idt_register_gate(34, (u32)irq2);
    idt_register_gate(35, (u32)irq3);
    idt_register_gate(36, (u32)irq4);
    idt_register_gate(37, (u32)irq5);
    idt_register_gate(38, (u32)irq6);
    idt_register_gate(39, (u32)irq7);
    idt_register_gate(40, (u32)irq8);
    idt_register_gate(41, (u32)irq9);
    idt_register_gate(42, (u32)irq10);
    idt_register_gate(43, (u32)irq11);
    idt_register_gate(44, (u32)irq12);
    idt_register_gate(45, (u32)irq13);
    idt_register_gate(46, (u32)irq14);
    idt_register_gate(47, (u32)irq15);

    idt_setup(); // Load with ASM
}


extern void page_fault(Intrrupt_mdata regs);

void isr_handler(Intrrupt_mdata r) {
    printf("received interrupt: %d\n",r.int_no);
    if(r.int_no == 14) page_fault(r);
    STOP;
}

inline void irq_register_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(Intrrupt_mdata r) {
    PIC_send_eoi(r.int_no);
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

#undef IDT_STATIC_IMPLEMENTATION_C
#undef PIC_STATIC_IMPLEMENTATION_C
#endif

