// @desc     Simple kernel.
// @author   Davide Della Giustina
// @date     07/12/2019

#include "../drivers/screen.h"

void kmain() {
    clear_screen();
    kprint("Hello world!\n");
    kprint_at("Test scroll\nLet's see...", 24, 0);
}