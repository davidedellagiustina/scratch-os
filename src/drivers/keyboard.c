// @desc     Keyboard driver
// @author   Davide Della Giustina
// @date     04/01/2020

#include "keyboard.h"

#define SC_MAX      57
#define BACKSPACE   0x0e
#define ENTER       0x1c

static char buff[256];

const char *sc_name[] = { "Error", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "+", "Backspace", "Tab", "q", "w", "e", "r",
    "t", "y", "u", "i", "o", "p", "'", "ì", "Enter", "LCtrl", "a", "s",
    "d", "f", "g", "h", "j", "k", "l", "è", "à", "ò", "LShift", "\\", "z",
    "x", "c", "v", "b", "n", "m", ",", ".", "ù", "RShift", "Keypad *", "LAlt",
    "Space" };
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '+', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '\'', (char)141, '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j',
    'k', 'l', (char)138, (char)133, (char)149, '?', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', (char)151, '?', '?', '?', ' ' };

/* Handler for the keyboard interrupts.
 * @param r             CPU state (registers).
 */
static void keyboard_callback(registers_t r) {
    uint8_t scancode = inb(SCANCODE_PORT);
    // /* DEBUG */ char sc[3]; itoa(scancode, sc); kprint(sc); kprint("\n");
    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {
        backspace(buff);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        parse_input(buff);
        buff[0] = '\0';
    } else {
        char c = sc_ascii[(int)scancode];
        char str[2] = {c, '\0'};
        append(buff, c);
        kprint(str);
    }
    UNUSED(r);
}

/* Initialize keryboard listener.
 */
void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}