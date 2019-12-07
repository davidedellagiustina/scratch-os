; @desc     Kernel entry: kall kmain label.
; @author   Davide Della Giustina
; @date     07/12/2019

[bits 32]
[extern kmain]

call kmain
jmp $