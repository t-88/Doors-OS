#include <stdint.h>
#include "screen.h"


#define VIDEO_MEM 0xb8000 
#define MAX_ROWS 25 
#define MAX_COLS 80 

void main() {
    uint8_t* vidMem = (uint8_t*) VIDEO_MEM;

    char* str = "C is Now Working!";
    int i = 0;
    while(str[i]) {
        vidMem[(2 *  MAX_COLS + 0) * 2 +  i * 2 + 0] = str[i]; 
        vidMem[(2 *  MAX_COLS + 0) * 2 +  i * 2 + 1] = 0x0f;
        i++;
    } 

}