#include "print.h"


#ifndef MACROS_H_
#define MACROS_H_


#define sti() asm volatile("sti");
#define cli() asm volatile("cli");

#define Unreachable(msg) do { \
    printf("[Unreachable] %s "__FILE__"\n",(msg)); STOP;\
} while (0);
 
#define PANIC(msg) do {\
    printf("[PANIC] %s "__FILE__"\n",(msg)); \
    STOP;\
} while (0);

#define STOP for (;;){}

#define ASSERT(cond) do {\
    if(!(cond)) { printf("[assertion failed]" __FILE__ ); STOP; } \
} while (0);



#endif