#ifndef GRAPHICS_H
#define GRAPHICS_H


// window
typedef struct window_t {
    bool grapped;
    int x , y;
    int w , h;
    u8 bg_color;
    
} window_t;


void window_init(window_t* window,int x , int y , int w ,int h) {
    window->grapped = false;
    window->x = x;
    window->y = y;
    window->w = w;
    window->h = h;
    window->bg_color = COLOR_YELLOW;
}


void  window_drag(window_t* window)  {
    static int x_offset = 0;
    static int y_offset = 0;

    if(window->grapped) {
        window->x = mouse_x - x_offset;
        window->y = mouse_y - y_offset;
        if(!mouse_left)  {
            window->grapped = false;
        }
    } else {
        if( mouse_left &&  AABB(window->x,window->y,window->w,window->h,mouse_x,mouse_y,2,2)) {
            window->grapped = true;
            x_offset = mouse_x - window->x;
            y_offset = mouse_y - window->y;
        }
    }
}

void window_draw(window_t window) {
    gfx_draw_rect(window.x,window.y,window.w,window.h,window.bg_color);
}



// button
bool button(x,y,w,h,c) {
    static bool mouse_left_pressed = false;
    gfx_draw_rect(x,y,w,h,c);

    if(mouse_left_pressed && !mouse_left) {
        mouse_left_pressed = false;
        return false;
    }
    if(!mouse_left_pressed && mouse_left && AABB(x,y,w,h,mouse_x,mouse_y,2,2)) {
        mouse_left_pressed = true;
        return true;

    }

    return false;
}
#endif