//TODO: Add Static implemtation aka #undef

//TODO: restart pagging
#define PAGING_IMPLEMENTATION_C
#include "paging.h"



#include "shared.h"

#define TIME_IMPLEMENTATION_C
#include "timer.h"


//TODO: restart mouse
// #define MOUSE_IMPLEMENTATION_C 
// #include "mouse.h"

#define TERMINAL_IMPLEMENETAION_C
#include "terminal.h"


#define MMAP_IMPLEMENTATION
#include "mmap.h"

#define VGA_IMPLEMENTATION
#include "vga.h"

#define KEYBOARD_IMPLEMENTATION_C
#include "keyboard.h"


#define PIC_IMPLEMENTATION_C
#define IDT_IMPLEMENTATION_C
#define ISR_IMPLEMENTATION_C
#include "isr.h"

#define SCREEN_IMPLEMENTATION_C
#define PRINT_IMPLEMENTATION_C
#include "print.h"

#define MEM_IMPLEMENTAION_C
#include "mem.h"

#define PORT_IMPLEMENTAION_C
#include "port.h"

#define MATH_IMPLEMENTATION_C
#include "math.h"

#define STRING_IMPLEMENTATION_C
#include "string.h"
