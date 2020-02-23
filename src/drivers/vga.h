// @desc     Screen driver header
// @author   Davide Della Giustina
// @date     07/12/2019

#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include "../cpu/ports.h"
#include "../libc/mem.h"

static const int VIDEO_ADDRESS = 0xc00b8000; // Virtual address for higher-half kernel (mapped to 0xb8000)

static const int MAX_ROWS = 25;
static const int MAX_COLS = 80;

static const int REG_SCREEN_CTRL = 0x3d4;
static const int REG_SCREEN_DATA = 0x3d5;

typedef enum vga_color {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    MAGENTA = 0x5,
    BROWN = 0x6,
    LIGHT_GREY = 0x7,
    DARK_GREY = 0x8,
    LIGHT_BLUE = 0x9,
    LIGHT_GREEN = 0xa,
    LIGHT_CYAN = 0xb,
    LIGHT_RED = 0xc,
    LIGHT_MAGENTA = 0xd,
    LIGHT_BROWN = 0xe,
    WHITE = 0xf,
    FG_DEFAULT = WHITE,
    BG_DEFAULT = BLACK
} vga_color;

// Public kernel API

/* Print a string at the specified cursor position.
 * @param msg       String to print.
 * @param row       Cursor row index.
 * @param col       Cursor column index.
 */
void kprint_at(char *msg, int row, int col);

/* Print a string at the current cursor position.
 * @param msg       String to print.
 */
void kprint(char *msg);

/* Clear the entire screen.
 */
void clear_screen();

/* Print a backspace (i.e. delete last char on the screen).
 */
void clear_last_char();

#endif