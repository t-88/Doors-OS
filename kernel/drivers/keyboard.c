#include "keyboard.h"
#include "isr.h"
#include "screen.h"

static bool keyboard_get_char_checked; 

void keyboard_print_key(Keyboard_Key key) {
    switch (key) {
        case ESC: kprint("ESC",-1); break;
        case NUM_1: kprint("NUM_1",-1); break;
        case NUM_2: kprint("NUM_2",-1); break;
        case NUM_3: kprint("NUM_3",-1); break;
        case NUM_4: kprint("NUM_4",-1); break;
        case NUM_5: kprint("NUM_5",-1); break;
        case NUM_6: kprint("NUM_6",-1); break;
        case NUM_7: kprint("NUM_7",-1); break;
        case NUM_8: kprint("NUM_8",-1); break;
        case NUM_9: kprint("NUM_9",-1); break;
        case NUM_0: kprint("NUM_0",-1); break;
        case Minus: kprint("Minus",-1); break;
        case Plus: kprint("Plus",-1); break;
        case Backspace: kprint("Backspace",-1); break;
        case Tab: kprint("Tab",-1); break;
        case Q: kprint("Q",-1); break;
        case W: kprint("W",-1); break;
        case E: kprint("E",-1); break;
        case R: kprint("R",-1); break;
        case T: kprint("T",-1); break;
        case Y: kprint("Y",-1); break;
        case U: kprint("U",-1); break;
        case I: kprint("I",-1); break;
        case O: kprint("O",-1); break;
        case P: kprint("P",-1); break;
        case SQR_PRA_OPEN: kprint("SQR_PRA_OPEN",-1); break;
        case SQR_PRA_CLOSE: kprint("SQR_PRA_CLOSE",-1); break;
        case Enter: kprint("Enter",-1); break;
        case LCtrl: kprint("LCtrl",-1); break;
        case A: kprint("A",-1); break;
        case S: kprint("S",-1); break;
        case D: kprint("D",-1); break;
        case F: kprint("F",-1); break;
        case G: kprint("G",-1); break;
        case H: kprint("H",-1); break;
        case J: kprint("J",-1); break;
        case K: kprint("K",-1); break;
        case L: kprint("L",-1); break;
        case SemiColon: kprint("SemiColon",-1); break;
        case Apostrophe: kprint("Apostrophe",-1); break;
        case BackTick: kprint("BackTick",-1); break;
        case LShift: kprint("LShift",-1); break;
        case BackSlash: kprint("BackSlash",-1); break;
        case Z: kprint("Z",-1); break;
        case X: kprint("X",-1); break;
        case C: kprint("C",-1); break;
        case V: kprint("V",-1); break;
        case B: kprint("B",-1); break;
        case N: kprint("N",-1); break;
        case M: kprint("M",-1); break;
        case Comma: kprint("Comma",-1); break;
        case Div: kprint("Div",-1); break;
        case Rshift: kprint("Rshift",-1); break;
        case Keypad: kprint("Keypad",-1); break;
        case LAlt: kprint("LAlt",-1); break;
        case Space: kprint("Space",-1); break;            
    }
}
void print_letter(u16 scancode) {
    // kprint("keyboard irq: ",-1);

    // error key comes in index 0 so we offset it

    if(scancode == 0) {
        // kprint("ERROR",-1);
    } else if(scancode >= 0x1 && scancode <= 0x39) {
        // keyboard_print_key(scancode - 1);
        keyboard_get_char_checked = true;

    } else if(scancode <= 0x7f) {
        // kprint("Unknown key down",-1);
    } else if (scancode <= 0x39 + 0x80) {
        // kprint("key up",-1);
        // keyboard_print_key(scancode - 1 - 0x80);
    }

    //TODO: maybe ignore unkown chars or just print them
    // well its reachable, arrow keys and stuff,
    // Unreachable(" unreachable code in print_letter ");
    // for (;;){}
    // kprint("\n",-1);

}


static void keyboard_callback(Intrrupt_mdata reg) {
    u8 scancode = port_byte_in(0x60);
    UN_USED(reg);
    UN_USED(scancode);


    print_letter(scancode);
}

void init_keyboard_driver() {
    irq_register_handler(IRQ1,keyboard_callback);
}

void keybaord_get_char() {
    keyboard_get_char_checked = false;
    while (!keyboard_get_char_checked) { }
}