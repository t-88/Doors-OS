#pragma once
#include <stdint.h>

#include "types.h"
#include "sys_std.h"


#define sti() asm volatile("sti");
#define UN_USED(val) (void) val;
#define Unreachable(msg) kprint(msg __FILE__,-1);