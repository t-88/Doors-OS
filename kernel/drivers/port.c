#include "port.h"


uint8_t port_byte_in(uint16_t port) {
    uint8_t in;
    asm("in %%dx, %%al" : "=a" (in) :"d" (port));
    return in;
}

void port_byte_out(uint16_t port,uint8_t data) {
    asm("out %%al , %%dx" : : "a" (data) ,"d" (port));
}


uint16_t port_word_in(uint16_t port) {
    uint16_t in;
    asm("in %%dx, %%ax" : "=a" (in) :"d" (port));
    return in;
}

void port_word_out(uint16_t port,uint16_t data) {
    asm("out %%ax , %%dx" : : "a" (data) ,"d" (port));
}

// used to wait for 1 to 4 ms, PIC relax timing for now
void io_wait(void){
    port_byte_out(0x80, 0);
}

