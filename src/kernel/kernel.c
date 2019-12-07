// @desc     Simple kernel.
// @author   Davide Della Giustina
// @date     07/12/2019

void kmain() {
    char *video_memory = (char *)0xb8000;
    *video_memory = 'X';
}