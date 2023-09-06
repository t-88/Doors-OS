#pragma once
#include <stdint.h>

#include "types.h"
#include "sys_std.h"

typedef void* type_t;

#define sti() asm volatile("sti");
#define UN_USED(val) (void) val;
#define Unreachable(msg) do { \
    kprint(msg " " __FILE__"\n",-1); STOP;\
} while (0);
 
#define PANIC(msg) do {\
    kprint(msg " "__FILE__ "\n",-1); \
    for (;;){}\
} while (0);

#define STOP for (;;){}
 