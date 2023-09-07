#include <stdarg.h>

#include "shared.h"
#include "sys_std.h"
#include "screen.h"


#ifndef PRINT_H_
#define PRINT_H_

#define PRINT_HEX(number) do {\
    char str[100];\
    hex_to_str(number,str);\
    kprints(str);\
} while (0);

#define PRINT_INT(number) do {\
    char str[100];\
    int_to_str(number,str);\
    kprints(str);\
} while (0);


#define PRINT(str) do {\
    kprints(str);\
} while (0);


void printf(const char* fmt,...);

#endif

// #define PRINT_IMPLEMENTATION_C 
#ifdef PRINT_IMPLEMENTATION_C
void printf(const char* fmt,...) {
    va_list ap;
    va_start(ap,fmt);


    char _char;
    while (*fmt) {
        _char = *fmt;
        fmt++;
        if(_char == '%') {
            if(*fmt == 'c') { 
                kprintc(va_arg(ap,char*));
            }
            else if(*fmt == 's') {
                kprints(va_arg(ap,char*));
            } 
            else if (*fmt == 'd')
            {
                PRINT_INT(va_arg(ap,int));
            } 
            else if (*fmt == 'x')
            {
                PRINT_HEX(va_arg(ap,long long int));
            } 
            else if(_char == '%' ) {
                kprintc(*fmt);
            }
            else {
                Unreachable("[printf type] unknowen type provided");
            }
            fmt++;
        } else {
            kprintc(_char);
        }
    }
    va_end(ap);
}

#endif