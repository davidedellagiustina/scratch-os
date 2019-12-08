// @desc     Screen driver header.
// @author   Davide Della Giustina
// @date     07/12/2019

#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

// Colors
#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define LIGHT_GREY 0x7
#define DARK_GREY 0x8
#define LIGHT_BLUE 0x9
#define LIGHT_GREEN 0xa
#define LIGHT_CYAN 0xb
#define LIGHT_RED 0xc
#define LIGHT_MAGENTA 0xd
#define LIGHT_BROWN 0xe
#define WHITE 0xf

#define COLOR_DEFAULT WHITE
#define BG_DEFAULT BLACK
#define ATTR_DEFAULT ((BG_DEFAULT << 4) + COLOR_DEFAULT)

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

#endif