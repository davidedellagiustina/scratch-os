extern void kprint();

void p_init_main() {
    while (1) kprint("INIT!\n");
}

void p_init_end() { return; }