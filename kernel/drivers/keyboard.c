#include "keyboard.h"
#include "isr.h"
#include "screen.h"

void print_letter(u16 scancode) {
    switch (scancode) {
        case 0x0:
            kprint("ERROR",-1);
            break;
        case 0x1:
            kprint("ESC",-1);
            break;
        case 0x2:
            kprint("1",-1);
            break;
        case 0x3:
            kprint("2",-1);
            break;
        case 0x4:
            kprint("3",-1);
            break;
        case 0x5:
            kprint("4",-1);
            break;
        case 0x6:
            kprint("5",-1);
            break;
        case 0x7:
            kprint("6",-1);
            break;
        case 0x8:
            kprint("7",-1);
            break;
        case 0x9:
            kprint("8",-1);
            break;
        case 0x0A:
            kprint("9",-1);
            break;
        case 0x0B:
            kprint("0",-1);
            break;
        case 0x0C:
            kprint("-",-1);
            break;
        case 0x0D:
            kprint("+",-1);
            break;
        case 0x0E:
            kprint("Backspace",-1);
            break;
        case 0x0F:
            kprint("Tab",-1);
            break;
        case 0x10:
            kprint("Q",-1);
            break;
        case 0x11:
            kprint("W",-1);
            break;
        case 0x12:
            kprint("E",-1);
            break;
        case 0x13:
            kprint("R",-1);
            break;
        case 0x14:
            kprint("T",-1);
            break;
        case 0x15:
            kprint("Y",-1);
            break;
        case 0x16:
            kprint("U",-1);
            break;
        case 0x17:
            kprint("I",-1);
            break;
        case 0x18:
            kprint("O",-1);
            break;
        case 0x19:
            kprint("P",-1);
            break;
		case 0x1A:
			kprint("[",-1);
			break;
		case 0x1B:
			kprint("]",-1);
			break;
		case 0x1C:
			kprint("ENTER",-1);
			break;
		case 0x1D:
			kprint("LCtrl",-1);
			break;
		case 0x1E:
			kprint("A",-1);
			break;
		case 0x1F:
			kprint("S",-1);
			break;
        case 0x20:
            kprint("D",-1);
            break;
        case 0x21:
            kprint("F",-1);
            break;
        case 0x22:
            kprint("G",-1);
            break;
        case 0x23:
            kprint("H",-1);
            break;
        case 0x24:
            kprint("J",-1);
            break;
        case 0x25:
            kprint("K",-1);
            break;
        case 0x26:
            kprint("L",-1);
            break;
        case 0x27:
            kprint(";",-1);
            break;
        case 0x28:
            kprint("'",-1);
            break;
        case 0x29:
            kprint("`",-1);
            break;
		case 0x2A:
			kprint("LShift",-1);
			break;
		case 0x2B:
			kprint("\\",-1);
			break;
		case 0x2C:
			kprint("Z",-1);
			break;
		case 0x2D:
			kprint("X",-1);
			break;
		case 0x2E:
			kprint("C",-1);
			break;
		case 0x2F:
			kprint("V",-1);
			break;
        case 0x30:
            kprint("B",-1);
            break;
        case 0x31:
            kprint("N",-1);
            break;
        case 0x32:
            kprint("M",-1);
            break;
        case 0x33:
            kprint(",",-1);
            break;
        case 0x34:
            kprint(".",-1);
            break;
        case 0x35:
            kprint("/",-1);
            break;
        case 0x36:
            kprint("Rshift",-1);
            break;
        case 0x37:
            kprint("Keypad *",-1);
            break;
        case 0x38:
            kprint("LAlt",-1);
            break;
        case 0x39:
            kprint("Spc",-1);
            break;
        default:
            // /* 'keuyp' event corresponds to the 'keydown' + 0x80 
            //  * it may still be a scancode we haven't implemented yet, or
            //  * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                kprint("Unknown key down",-1);
            } else if (scancode <= 0x39 + 0x80) {
                kprint("key up ",-1);
                print_letter(scancode - 0x80);
            } else kprint("Unknown key up",-1);
            break;
    }
}


static void keyboard_callback(registers_t reg) {
    u8 scancode = port_byte_in(0x60);
    kprint("keyboard irq: ",-1);
    print_letter(scancode);
    kprint("\n",-1);
}



void init_keyboard_driver() {
    register_interrupt_handler(33,keyboard_callback);
}