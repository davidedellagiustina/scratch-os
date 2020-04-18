extern void kprint();

void p_init_main() {
    asm volatile("l: jmp l");
    while (1) kprint("INIT!\n");
}

void p_init_end() { return; }