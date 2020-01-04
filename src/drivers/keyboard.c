// @desc     Keyboard driver
// @author   Davide Della Giustina
// @date     04/01/2020

#include "keyboard.h"

// Private functions
void print_letter(uint8_t scancode);

/* Handler for the keyboard interrupts.
 * @param r             CPU state (registers).
 */
static void keyboard_callback(registers_t r) {
    uint8_t scancode = inb(SCANCODE_PORT);
    char *sc_ascii;
    int_to_ascii(scancode, sc_ascii);
    kprint("Keyboard scancode: ");
    kprint(sc_ascii);
    kprint(", ");
    print_letter(scancode);
    kprint("\n");
}

/* Initialize keryboard listener.
 */
void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

/* Print the letter corresponding to the received scancode.
 * Keyboard layout: italian.
 * @param scancode          Scancode received from the PIC.
 */
void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0: // Error
            kprint("ERROR");
            break;
        case 0x1: // Esc
            kprint("Esc");
            break;
        case 0x2: // 1
            kprint("1");
            break;
        case 0x3: // 2
            kprint("2");
            break;
        case 0x4: // 3
            kprint("3");
            break;
        case 0x5: // 4
            kprint("4");
            break;
        case 0x6: // 5
            kprint("5");
            break;
        case 0x7: // 6
            kprint("6");
            break;
        case 0x8: // 7
            kprint("7");
            break;
        case 0x9: // 8
            kprint("8");
            break;
        case 0xa: // 9
            kprint("9");
            break;
        case 0xb: // 0
            kprint("0");
            break;
        case 0xc: // '
            kprint("'");
            break;
        case 0xd: // ì
            kprint("ì");
            break;
        case 0xe: // Backspace
            kprint("Backspace");
            break;
        case 0xf: // Tab
            kprint("Tab");
            break;
        case 0x10: // Q
            kprint("Q");
            break;
        case 0x11: // W
            kprint("W");
            break;
        case 0x12: // E
            kprint("E");
            break;
        case 0x13: // R
            kprint("R");
            break;
        case 0x14: // T
            kprint("T");
            break;
        case 0x15: // Y
            kprint("Y");
            break;
        case 0x16: // U
            kprint("U");
            break;
        case 0x17: // I
            kprint("I");
            break;
        case 0x18: // O
            kprint("O");
            break;
        case 0x19: // P
            kprint("P");
            break;
        case 0x1a: // è
            kprint("è");
            break;
        case 0x1b: // +
            kprint("+");
            break;
        case 0x1c: // Enter
            kprint("Enter");
            break;
        case 0x1d: // LCtrl
            kprint("LCtrl");
            break;
        case 0x1e: // A
            kprint("A");
            break;
        case 0x1f: // S
            kprint("S");
            break;
        case 0x20: // D
            kprint("D");
            break;
        case 0x21: // F
            kprint("F");
            break;
        case 0x22: // G
            kprint("G");
            break;
        case 0x23: // H
            kprint("H");
            break;
        case 0x24: // J
            kprint("J");
            break;
        case 0x25: // K
            kprint("K");
            break;
        case 0x26: // L
            kprint("L");
            break;
        case 0x27: // ò
            kprint("ò");
            break;
        case 0x28: // à
            kprint("à");
            break;
        case 0x29: // '\'
            kprint("\\");
            break;
        case 0x2a: // LShift
            kprint("LShift");
            break;
        case 0x2b: // ù
            kprint("ù");
            break;
        case 0x2c: // Z
            kprint("Z");
            break;
        case 0x2d: // X
            kprint("X");
            break;
        case 0x2e: // C
            kprint("C");
            break;
        case 0x2f: // V
            kprint("V");
            break;
        case 0x30: // B
            kprint("B");
            break;
        case 0x31: // N
            kprint("N");
            break;
        case 0x32: // M
            kprint("M");
            break;
        case 0x33: // ,
            kprint(",");
            break;
        case 0x34: // .
            kprint(".");
            break;
        case 0x35: // -
            kprint("-");
            break;
        case 0x36: // RShift
            kprint("RShift");
            break;
        case 0x37: // Keypad *
            kprint("Keypad *");
            break;
        case 0x38: // LAlt
            kprint("LAlt");
            break;
        case 0x39: // Space
            kprint("Space");
            break;
        default:
            // 'keyup' event is scancode + 0x80
            // So here it may be an escape sequence, an unimplemented scancode or a 'keyup' event
            if (scancode < KEYUP_EVENT_OFFSET)
                kprint("Unknown key down");
            else if (scancode <= 0x39 + KEYUP_EVENT_OFFSET) {
                kprint("key up ");
                print_letter(scancode - KEYUP_EVENT_OFFSET);
            } else 
                kprint("Unknown key up");
            break;
    }
}