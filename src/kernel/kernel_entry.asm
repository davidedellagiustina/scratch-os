; @desc     Kernel entry: kall kmain()
; @author   Davide Della Giustina
; @date     07/12/2019

[bits 32]
[extern kmain]

call kmain
jmp $ ; Control should never get back there, just for prevention