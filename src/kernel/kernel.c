// @desc     Simple kernel.
// @author   Davide Della Giustina
// @date     07/12/2019

#include "../drivers/vga.h"

void kmain() {
    clear_screen();
    kprint("Hello world!\n");
}