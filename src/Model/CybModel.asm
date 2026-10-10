.section .rodata
.align 4

.global uCYModelBegin
.global uCYModelEnd

// Please use level 0 model for 48MHz M0/M23 MCU.

uCYModelBegin:
.incbin "../src/Model/Test2_pack_WithTxt.bin"
uCYModelEnd:

