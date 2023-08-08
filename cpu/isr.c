
#include "isr.h"
#include "sys_std.h"
#include "pic.h"
#include "paging.h"

extern void page_fault(Intrrupt_mdata regs);

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

void isr_handler(Intrrupt_mdata r) {
    kprint("received interrupt: ",-1);
    char str[5];
    int_to_str(r.int_no,str);
    kprint(str,-1);
    kprint("\n",-1);

    if(r.int_no == 14) {
        page_fault(r);
    }
}


isr_t interrupt_handlers[256];
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