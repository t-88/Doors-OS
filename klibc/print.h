#ifndef PRINT_H_
#define PRINT_H_
#include <stdarg.h>

#include "shared.h"
#include "string.h"
#include "screen.h"


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
#include <stdarg.h>

#include "shared.h"
#include "string.h"
#include "screen.h"

#define PRINT_INT_TYPES(type) do{\
    char buffer[256];\
    int_to_str(va_arg(ap,type),buffer); \
    int len = strlen(buffer);\
    if(number > len) {\
        for (u8 i = 0; i < number - len; i++) {\
            printf("0");\
        }\
    }  \
    printf("%s",buffer);\
} while (0);


#define PRINT_HEX_TYPES(type) do{\
    char buffer[256];\
    int_to_hex_str(va_arg(ap,type),buffer); \
    int len = strlen(buffer);\
    if(number > len) {\
        for (u8 i = 0; i < number - len; i++) {\
            printf("0");\
        }\
    }  \
    printf("%s",buffer);\
} while (0);


void printf(const char* fmt,...) {
    va_list ap;
    va_start(ap,fmt);


    char _char;
    while (*fmt) {
        _char = *fmt;
        fmt++;
        if(_char == '%') {
            // NOTE: Supports only 20 numbers for now
            char number_buffer[20];
            u8 number_buffer_i = 0;
            while (isnumeric(*fmt)) {
                number_buffer[number_buffer_i++] = *fmt++;
            }
            number_buffer[number_buffer_i] = '\0';
            int number = str_to_int(number_buffer);
            

            if(*fmt == '*' && number_buffer_i == 0) {
                int size = va_arg(ap,int);
                for(u8 i = 0; i < size; i++) {
                    printf(" ");
                }
                fmt++;
            }


            if(*fmt == 'c') { 
                kprintc(va_arg(ap,char*));
            }
            else if(*fmt == 's') {
                if(number_buffer_i != 0) {
                    kprints_sized(va_arg(ap,char*),number);
                } else {
                    kprints(va_arg(ap,char*));
                }
            } 
            else if(*fmt == 'l') {
                fmt++;
                _char = *fmt;
                if (_char == 'l') {
                    Unreachable("[printf error] Triple Long is not supported");
                } else if (_char == 'd') {
                    PRINT_INT_TYPES(long);
                } else if (_char == 'x') {
                    PRINT_HEX_TYPES(long long);
                }
            }
            else if (*fmt == 'd') {
                PRINT_INT_TYPES(int);
            } 
            else if (*fmt == 'x') {
                PRINT_HEX_TYPES(int);
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