#include "pic.h"

void PIC_send_eoi(u8 irq) {
    if(irq >= PIC2_OFFSET)
        port_byte_out(PIC2_CMD,PIC_CMD_EOF);
    port_byte_out(PIC1_CMD,PIC_CMD_EOF);
}


void PIC_remap(int offset1,int offset2) {
    u8 a1,a2;

    a1 = port_byte_in(PIC1_DATA);
    a2 = port_byte_in(PIC2_DATA);

    port_byte_out(PIC1_CMD,ICW1_INIT | ICW1_ICW4); io_wait();
    port_byte_out(PIC2_CMD,ICW1_INIT | ICW1_ICW4); io_wait();
    port_byte_out(PIC1_DATA,offset1); io_wait();
    port_byte_out(PIC2_DATA,offset2); io_wait();
    port_byte_out(PIC1_DATA,4); io_wait();
    port_byte_out(PIC2_DATA,2); io_wait();
    port_byte_out(PIC1_DATA,ICW4_8086); io_wait();
    port_byte_out(PIC2_DATA,ICW4_8086); io_wait();

    port_byte_out(PIC1_DATA,a1);
    port_byte_out(PIC2_DATA,a2);
}
