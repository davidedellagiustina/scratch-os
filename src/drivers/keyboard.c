// @desc     Keyboard driver
// @author   Davide Della Giustina
// @date     04/01/2020

#include "keyboard.h"

#define SCANCODE_PORT       0x60
#define KEYUP_EVENT_OFFSET  0x80
#define SC_MAX              57
#define BACKSPACE           0x0e
#define ENTER               0x1c
#define LSHIFT              0x2a
#define RSHIFT              0x36
#define LCTRL               0x1d
#define LALT                0x38

static char buff[1024];
int shift_pressed = 0;
int ctrl_pressed = 0;
int alt_pressed = 0;

const char layout[] = "italian";
const char keys[] = { '?' /* Error */, '?' /* Esc */, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', '?' /* Backspace */, '\t' /* Tab */, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '\'', (char)141 /* ì */, '?' /* Enter */, '?' /* LCtrl */, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', (char)138 /* è */, (char)133 /* à */, (char)149 /* ò */, '?' /* LShift */, '\\', 'z', 'x', 'c',
    'v', 'b', 'n', 'm', ',', '.', (char)151 /* ù */, '?' /* RShift */, '?' /* Keypad * */, '?' /* LAlt */, ' ' /* Space */ };
const char keys_shift[] = { '?' /* Error */, '?' /* Esc */, '!', '"', (char)156 /* £ */, '$', '%', '&', '/', '(', ')', '=', '_', '*', '?' /* Backspace */, '\t' /* Tab */, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O',
    'P', '?' /* ? */, '^', '?' /* Enter */, '?' /* LCtrl */, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', (char)130 /* é */, (char)167 /* ° */, (char)135 /* ç */, '?' /* LShift */, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':',
    (char)245 /* § */, '?' /* RShift */, '?' /* Keypad * */, '?' /* LAlt */, ' ' /* Space */ };

/* Handler for the keyboard interrupts.
 * @param r             CPU state (registers).
 */
static void keyboard_callback(registers_t *r) {
    uint8_t scancode = inb(SCANCODE_PORT);
    // Special keys (shift, ctl, alt)
    if (scancode == LSHIFT || scancode == RSHIFT) shift_pressed = 1;
    else if (scancode == LSHIFT + KEYUP_EVENT_OFFSET || scancode == RSHIFT + KEYUP_EVENT_OFFSET) shift_pressed = 0;
    else if (scancode == LCTRL) ctrl_pressed = 1;
    else if (scancode == LCTRL + KEYUP_EVENT_OFFSET) ctrl_pressed = 0;
    else if (scancode == LALT) alt_pressed = 1;
    else if (scancode == LALT + KEYUP_EVENT_OFFSET) alt_pressed = 0;
    // Unhandled scancodes
    else if (scancode > SC_MAX) return;
    // Backspace
    else if (scancode == BACKSPACE) {
        str_truncate(buff, 1);
        clear_last_char();
    // Enter
    } else if (scancode == ENTER) {
        kprint("\n");
        parse_input(buff);
        buff[0] = '\0';
    // Other characters
    } else {
        char c;
        if (shift_pressed) c = keys_shift[(int)scancode];
        else c = keys[(int)scancode];
        char str[2] = { c, '\0' };
        strncat(buff, &c, 1);
        kprint(str);
    }
    (void)(layout); // Unused parameter
    (void)(r); // Unused parameter
}

/* Initialize keryboard listener.
 */
void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}