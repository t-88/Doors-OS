#pragma once
#include "types.h"

extern bool mouse_down;
extern bool mouse_left;
extern bool mouse_right;
extern bool mouse_middle;

extern int dir_x;
extern int dir_y;
extern int mouse_x_delta;
extern int mouse_y_delta;

extern float mouse_x;
extern float mouse_y;

extern void mouse_init_driver();
extern void mouse_print_debug();