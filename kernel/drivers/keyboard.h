#include "shared.h"
#include "isr.h"

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

typedef enum Keyboard_Key {
    ESC,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,
    NUM_0,
    Minus,
    Plus,
    Backspace,
    Tab,
    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    O,
    P,
    SQR_PRA_OPEN,
    SQR_PRA_CLOSE,
    Enter,
    LCtrl,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    SemiColon,
    Apostrophe,
    BackTick,
    LShift,
    BackSlash,
    Z,
    X,
    C,
    V,
    B,
    N,
    M,
    Comma,
    Div,
    Rshift,
    Keypad,
    LAlt,
    // TODO: what is scancode 55
    _,
    Space,
} Keyboard_Key;


static bool keyboard_wait_char_down; 
static bool keyboard_wait_char_up; 
static char last_char;


#define KEYBOARD_CHAR_BUFFER_MAX 20
char keyboard_char_buffer[KEYBOARD_CHAR_BUFFER_MAX];
int keyboard_char_buffer_index = 0;

void init_keyboard_driver();
static void keyboard_callback(Intrrupt_mdata reg);

void keyboard_print_char(u16 scancode);
void keybaord_wait_char(const char _char);
void keyboard_key_down(u32 scancode);
void keyboard_key_up(u32 scancode);

void keyboard_push_to_buffer(u32 scancode);
const char keyboard_pop_from_buffer();

#endif

// #define KEYBOARD_IMPLEMENTATION_C
#ifdef KEYBOARD_IMPLEMENTATION_C

void init_keyboard_driver() {
    irq_register_handler(IRQ1,keyboard_callback);
    keyboard_char_buffer_index = 0;
    keyboard_wait_char_down = false;
    keyboard_wait_char_up = false;
    last_char = 0;
}

static u32 keyboard_char_to_scanecode(const char _char) {
    switch (_char) {
        case 27:           return ESC; 
        case '1':         return NUM_1; 
        case '2':         return NUM_2; 
        case '3':         return NUM_3; 
        case '4':         return NUM_4; 
        case '5':         return NUM_5; 
        case '6':         return NUM_6; 
        case '7':         return NUM_7; 
        case '8':         return NUM_8; 
        case '9':         return NUM_9; 
        case '0':         return NUM_0; 
        case '-':         return Minus; 
        case '+':         return Plus; 
        case 8:           return Backspace; 
        case 9:           return Tab; 
        case 'q':             return Q;
        case 'w':             return W;
        case 'e':             return E;
        case 'r':             return R;
        case 't':             return T;
        case 'y':             return Y;
        case 'u':             return U;
        case 'i':             return I;
        case 'o':             return O;
        case 'p':             return P;
        case '[':           return SQR_PRA_OPEN ; 
        case ']':           return SQR_PRA_CLOSE;
        case '\n':         return Enter;
        case 'a':             return A;
        case 's':             return S;
        case 'd':             return D;
        case 'f':             return F;
        case 'g':             return G;
        case 'h':             return H;
        case 'j':             return J;
        case 'k':             return K;
        case 'l':             return L;
        case ';' :     return SemiColon  ; 
        case '\'' :    return Apostrophe ;
        case '`' :      return BackTick   ; 
        case '\\':     return BackSlash  ;
        case 'z' :             return Z          ;
        case 'x' :             return X          ;
        case 'c' :             return C          ;
        case 'v' :             return V          ;
        case 'b' :             return B          ;
        case 'n' :             return N          ;
        case 'm' :             return M          ;
        case ',' :         return Comma      ; 
        case '/' :           return Div        ; 
        case ' ':         return Space       ;
        // case '\0':         return LCtrl;
        // case '\0':        return Rshift     ;
        // case '\0':        return LShift     ;
        // case '\0':        return Keypad     ;
        // case '\0':          return LAlt       ;
    }  
    PANIC("some chars are not implemented yet");
}
static const char keyboard_scanecode_to_char(u32 code) {
    switch (code) {
        case ESC:       return   27; 
        case NUM_1:     return  '1'; 
        case NUM_2:     return  '2'; 
        case NUM_3:     return  '3'; 
        case NUM_4:     return  '4'; 
        case NUM_5:     return  '5'; 
        case NUM_6:     return  '6'; 
        case NUM_7:     return  '7'; 
        case NUM_8:     return  '8'; 
        case NUM_9:     return  '9'; 
        case NUM_0:     return  '0'; 
        case Minus:     return  '-'; 
        case Plus:      return  '+'; 
        case Backspace: return 8; 
        case Tab: return 9; 
        case Q: return 'q';
        case W: return 'w';
        case E: return 'e';
        case R: return 'r';
        case T: return 't';
        case Y: return 'y';
        case U: return 'u';
        case I: return 'i';
        case O: return 'o';
        case P: return 'p';
        case SQR_PRA_OPEN:  return '['; 
        case SQR_PRA_CLOSE: return ']';
        case Enter:  return 10;
        case LCtrl: return '\0';
        case A: return 'a';
        case S: return 's';
        case D: return 'd';
        case F: return 'f';
        case G: return 'g';
        case H: return 'h';
        case J: return 'j';
        case K: return 'k';
        case L: return 'l';
        case SemiColon: return ';'; 
        case Apostrophe: return ',';
        case BackTick: return '`'; 
        case LShift: return '\0';
        case BackSlash: return '\\';
        case Z: return 'z';
        case X: return 'x';
        case C: return 'c';
        case V: return 'v';
        case B: return 'b';
        case N: return 'n';
        case M: return 'm';
        case Comma: return ','; 
        case Div:   return '/'; 
        case Rshift: return '\0';
        case Keypad: return '\0';
        case LAlt: return '\0';
        case Space: return ' ';
        
        default: break;
    }
    PANIC("some scancodes are not implemented yet");
}


void keyboard_push_to_buffer(u32 scancode) {
    if(keyboard_char_buffer_index == KEYBOARD_CHAR_BUFFER_MAX) {
        for (u32 i = 1; i < KEYBOARD_CHAR_BUFFER_MAX; i++) {
            keyboard_char_buffer[keyboard_char_buffer_index - 1] = keyboard_char_buffer[keyboard_char_buffer_index];         
        }
    }
    
    keyboard_char_buffer[keyboard_char_buffer_index++] = keyboard_scanecode_to_char(scancode);

    if(keyboard_char_buffer_index > KEYBOARD_CHAR_BUFFER_MAX) {
        keyboard_char_buffer_index = KEYBOARD_CHAR_BUFFER_MAX;
    }
}

const char keyboard_pop_from_buffer() {
    if(keyboard_char_buffer_index == 0) {
        return 0;
    }
    
    return keyboard_char_buffer[--keyboard_char_buffer_index];
}


void keyboard_print_key(Keyboard_Key key) {
    switch (key) {
        case ESC: printf("ESC"); break;
        case NUM_1: printf("NUM_1"); break;
        case NUM_2: printf("NUM_2"); break;
        case NUM_3: printf("NUM_3"); break;
        case NUM_4: printf("NUM_4"); break;
        case NUM_5: printf("NUM_5"); break;
        case NUM_6: printf("NUM_6"); break;
        case NUM_7: printf("NUM_7"); break;
        case NUM_8: printf("NUM_8"); break;
        case NUM_9: printf("NUM_9"); break;
        case NUM_0: printf("NUM_0"); break;
        case Minus: printf("Minus"); break;
        case Plus: printf("Plus"); break;
        case Backspace: printf("Backspace"); break;
        case Tab: printf("Tab"); break;
        case Q: printf("Q"); break;
        case W: printf("W"); break;
        case E: printf("E"); break;
        case R: printf("R"); break;
        case T: printf("T"); break;
        case Y: printf("Y"); break;
        case U: printf("U"); break;
        case I: printf("I"); break;
        case O: printf("O"); break;
        case P: printf("P"); break;
        case SQR_PRA_OPEN: printf("SQR_PRA_OPEN"); break;
        case SQR_PRA_CLOSE: printf("SQR_PRA_CLOSE"); break;
        case Enter: printf("Enter"); break;
        case LCtrl: printf("LCtrl"); break;
        case A: printf("A"); break;
        case S: printf("S"); break;
        case D: printf("D"); break;
        case F: printf("F"); break;
        case G: printf("G"); break;
        case H: printf("H"); break;
        case J: printf("J"); break;
        case K: printf("K"); break;
        case L: printf("L"); break;
        case SemiColon: printf("SemiColon"); break;
        case Apostrophe: printf("Apostrophe"); break;
        case BackTick: printf("BackTick"); break;
        case LShift: printf("LShift"); break;
        case BackSlash: printf("BackSlash"); break;
        case Z: printf("Z"); break;
        case X: printf("X"); break;
        case C: printf("C"); break;
        case V: printf("V"); break;
        case B: printf("B"); break;
        case N: printf("N"); break;
        case M: printf("M"); break;
        case Comma: printf("Comma"); break;
        case Div: printf("Div"); break;
        case Rshift: printf("Rshift"); break;
        case Keypad: printf("Keypad"); break;
        case LAlt: printf("LAlt"); break;
        case Space: printf("Space"); break; 
        default: break;
    }
}
void keyboard_print_char(u16 scancode) {
    if(scancode == 0) {
        printf("ERROR\n");
    } else if(scancode >= 0x1 && scancode <= 0x39) {
        // keydown
        keyboard_wait_char_down = true;
        last_char = scancode - 1;
    } else if(scancode <= 0x7f) {
        printf("Unknown key down\n");
    } else if (scancode <= 0x39 + 0x80) {
        // keyup
        keyboard_wait_char_up = true;
        last_char = scancode - 1 - 0x80;
    }

    // TODO: maybe ignore unkown chars or just print them
    // well its reachable, arrow keys and stuff,
    // Unreachable(" unreachable code in keyboard_print_char ");
    // for (;;){}
    // kprint("\n",-1);

}


static void keyboard_callback(Intrrupt_mdata reg) {
    u8 scancode = port_byte_in(0x60);

    if(scancode >= 0x1 && scancode <= 0x39) {
        keyboard_key_down(scancode - 1);
    } else if (0x7f <  scancode && scancode <= 0x39 + 0x80) {
        keyboard_key_up(scancode - 1 - 0x80);
    }

    // keyboard_print_char(scancode);
}


void keyboard_key_down(u32 scancode) {
    keyboard_wait_char_down = true;
    last_char = scancode;

    keyboard_push_to_buffer(scancode);    
}
void keyboard_key_up(u32 scancode) {
    keyboard_wait_char_up = true;
    last_char = scancode;
}


void keybaord_wait_char(const char _char) {
    keyboard_wait_char_down = false;
    keyboard_wait_char_up = false;
    while (!keyboard_wait_char_down || last_char != keyboard_char_to_scanecode(_char)) {}
    while (!keyboard_wait_char_up || last_char != keyboard_char_to_scanecode(_char)) {}
}
#endif