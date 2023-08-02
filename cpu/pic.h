#pragma once
#include "shared.h"
#include "port.h"

#define PIC1 0x20
#define PIC1_CMD PIC1
#define PIC1_DATA 0x21

#define PIC2 0xA0
#define PIC2_CMD PIC2
#define PIC2_DATA 0xA1


#define PIC1_OFFSET 32
#define PIC2_OFFSET 40


#define PIC_CMD_EOF 0x20


#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LVL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10


extern void PIC_send_eoi(u8 irq);
extern void PIC_remap(int offset1,int offset2);


