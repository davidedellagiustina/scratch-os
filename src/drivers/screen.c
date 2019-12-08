// @desc     Screen driver.
// @author   Davide Della Giustina
// @date     07/12/2019

#include "../kernel/utils.h"
#include "../kernel/ports.h"
#include "screen.h"

// Private functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int row, int col);
int get_offset_row(int offset);
int get_offset_col(int offset);
int handle_scrolling(int offset);
int print_char(char character, int row, int column, char color, char bg_color);

/* Print a string at the specified cursor position.
 * @param msg       String to print.
 * @param row       Cursor row index.
 * @param col       Cursor column index.
 */
void kprint_at(char *msg, int row, int col) {
    if (row < 0 || col < 0) {
        int offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
    int i = 0;
    while (msg[i] != 0) {
        int offset = print_char(msg[i++], row, col, COLOR_DEFAULT, BG_DEFAULT);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

/* Print a string at the current cursor position.
 * @param msg       String to print.
 */
void kprint(char *msg) {
    kprint_at(msg, -1, -1);
}

/* Clear the entire screen.
 */
void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    unsigned char *screen = (unsigned char *)VIDEO_ADDRESS;
    int i;
    for (i = 0; i < screen_size; ++i) {
        screen[2*i] = ' ';
        screen[2*i+1] = ATTR_DEFAULT;
    }
    set_cursor_offset(get_offset(0, 0));
}

/* Get current cursor position as offset in memory.
 * @return          Current cursor position (memory offset).
 */
int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14); // Request higher byte
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15); // Request lower byte
    offset += port_byte_in(REG_SCREEN_DATA);
    return (2 * offset);
}

/* Set the cursor position.
 * @param offset    Cursor position (memory offset).
 */
void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

/* Compute memory offset basing on row/col indexes.
 * @param row       Row index.
 * @param col       Column index.
 * @return          Memory offset.
 */
int get_offset(int row, int col) {
    return (2 * (row * MAX_COLS + col));
}

/* Compute row index basing on memory offset.
 * @param offset    Memory offset.
 * @return          Row index.
 */
int get_offset_row(int offset) {
    return (offset / (2 * MAX_COLS));
}

/* Compute column index basing on memory offset.
 * @param offset    Memory offset.
 * @return          Column index.
 */
int get_offset_col(int offset) {
    return ((offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2);
}

/* Handle screen scrolling.
 * @param offset    Current cursor position (after writing a character).
 * @return          Corrected cursor position.
 */
int handle_scrolling(int offset) {
    if (offset >= 2 * MAX_ROWS * MAX_COLS) {
        int i;
        for (i = 1; i < MAX_ROWS; ++i) memory_copy((char *)(VIDEO_ADDRESS + get_offset(i, 0)), (char *)(VIDEO_ADDRESS + get_offset(i-1, 0)), 2*MAX_COLS);
        char *last_line = (char *)VIDEO_ADDRESS + get_offset(MAX_ROWS-1, 0);
        for (i = 0; i < MAX_COLS; i += 2) {
            last_line[i] = 0;
            last_line[i+1] = ATTR_DEFAULT;
        }
        offset -= 2 * MAX_COLS;
    }
    return offset;
}

/* Print a single char to the screen.
 * @param character Character to print.
 * @param row       Row index.
 * @param col       Column index.
 * @param color     Text color.
 * @param bg_color  Background color
 */
int print_char(char character, int row, int col, char color, char bg_color) {
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;
    char attribute = (bg_color << 4) + color;
    attribute = ((attribute == 0)? ATTR_DEFAULT : attribute);
    int offset = ((row >= 0 && col >= 0)? get_offset(row, col) : get_cursor_offset());
    if (character == '\n') {
        offset = get_offset(get_offset_row(offset), MAX_COLS-1);
    } else {
        vidmem[offset] = character;
        vidmem[offset+1] = attribute;
    }
    offset += 2;
    offset = handle_scrolling(offset);
    set_cursor_offset(offset);
    return offset;
}