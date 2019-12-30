// @desc     Screen driver.
// @author   Davide Della Giustina
// @date     07/12/2019

#include "vga.h"

// Private functions
uint8_t get_attribute(vga_color fg, vga_color bg);
uint32_t get_cursor_offset();
void set_cursor_offset(uint32_t offset);
uint32_t get_offset(int32_t row, int32_t col);
int32_t get_offset_row(uint32_t offset);
int32_t get_offset_col(uint32_t offset);
uint32_t handle_scrolling(uint32_t offset);
uint32_t print_char(char character, int32_t row, int32_t col, vga_color fg, vga_color bg);

/* Print a string at the specified cursor position.
 * @param msg       String to print.
 * @param row       Cursor row index.
 * @param col       Cursor column index.
 */
void kprint_at(char *msg, int32_t row, int32_t col) {
    if (row < 0 || col < 0) {
        uint32_t offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
    uint32_t i = 0;
    while (msg[i] != 0) {
        uint32_t offset = print_char(msg[i++], row, col, FG_DEFAULT, BG_DEFAULT);
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
    uint32_t screen_size = MAX_COLS * MAX_ROWS;
    unsigned char *screen = (unsigned char *)VIDEO_ADDRESS;
    uint32_t i;
    for (i = 0; i < screen_size; ++i) {
        screen[2*i] = ' ';
        screen[2*i+1] = get_attribute(FG_DEFAULT, BG_DEFAULT);
    }
    set_cursor_offset(get_offset(0, 0));
}

/* Build attribute basing con foreground and background color.
 * @param fg        Foreground color.
 * @param bg        Background color.
 * @return          Attribute.
 */
uint8_t get_attribute(vga_color fg, vga_color bg) {
    return (((uint8_t)bg << 4) | (uint8_t)fg);
}

/* Get current cursor position as offset in memory.
 * @return          Current cursor position (memory offset).
 */
uint32_t get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14); // Request higher byte
    uint32_t offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15); // Request lower byte
    offset += port_byte_in(REG_SCREEN_DATA);
    return (2 * offset);
}

/* Set the cursor position.
 * @param offset    Cursor position (memory offset).
 */
void set_cursor_offset(uint32_t offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

/* Compute memory offset basing on row/col indexes.
 * @param row       Row index.
 * @param col       Column index.
 * @return          Memory offset.
 */
uint32_t get_offset(int32_t row, int32_t col) {
    return (uint32_t)(2 * (row * MAX_COLS + col));
}

/* Compute row index basing on memory offset.
 * @param offset    Memory offset.
 * @return          Row index.
 */
int32_t get_offset_row(uint32_t offset) {
    return (int32_t)(offset / (2 * MAX_COLS));
}

/* Compute column index basing on memory offset.
 * @param offset    Memory offset.
 * @return          Column index.
 */
int32_t get_offset_col(uint32_t offset) {
    return (int32_t)((offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2);
}

/* Handle screen scrolling.
 * @param offset    Current cursor position (after writing a character).
 * @return          Corrected cursor position.
 */
uint32_t handle_scrolling(uint32_t offset) {
    if (offset >= 2 * MAX_ROWS * MAX_COLS) {
        uint32_t i;
        for (i = 1; i < MAX_ROWS; ++i) memory_copy((unsigned char *)(VIDEO_ADDRESS + get_offset(i, 0)), (unsigned char *)(VIDEO_ADDRESS + get_offset(i-1, 0)), 2*MAX_COLS);
        unsigned char *last_line = (unsigned char *)VIDEO_ADDRESS + get_offset(MAX_ROWS-1, 0);
        for (i = 0; i < MAX_COLS; i += 2) {
            last_line[i] = 0;
            last_line[i+1] = get_attribute(FG_DEFAULT, BG_DEFAULT);
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
 * @return          Position (offset) of the cursor (incremented by 1).
 */
uint32_t print_char(char character, int32_t row, int32_t col, vga_color fg, vga_color bg) {
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;
    unsigned char attribute = get_attribute(fg, bg);
    attribute = ((attribute == 0)? get_attribute(FG_DEFAULT, BG_DEFAULT) : attribute);
    uint32_t offset = ((row >= 0 && col >= 0)? get_offset(row, col) : get_cursor_offset());
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