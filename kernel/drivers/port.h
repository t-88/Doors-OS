#ifndef PORT_H_
#define PORT_H_

#include "shared.h"


uint8_t port_byte_in(uint16_t port);
void port_byte_out(uint16_t port,uint8_t data);

uint16_t port_word_in(uint16_t port);
void port_word_out(uint16_t port,uint16_t data);



uint8_t port_byte_in(uint16_t port) {
    uint8_t in;
    __asm__("in %%dx, %%al" : "=a" (in) :"d" (port));
    return in;
}

void port_byte_out(uint16_t port,uint8_t data) {
    __asm__("out %%al , %%dx" : : "a" (data) ,"d" (port));
}


uint16_t port_word_in(uint16_t port) {
    uint16_t in;
    __asm__("in %%dx, %%ax" : "=a" (in) :"d" (port));
    return in;
}

void port_word_out(uint16_t port,uint16_t data) {
    __asm__("out %%ax , %%dx" : : "a" (data) ,"d" (port));
}


#endif //PORT_H_