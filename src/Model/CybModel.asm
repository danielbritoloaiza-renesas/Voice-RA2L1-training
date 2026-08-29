.section .rodata
.align 4

.global uCYModelBegin
.global uCYModelEnd

// Please use level 0 model for 48MHz M0/M23 MCU.

uCYModelBegin:
.incbin "../src/Model/HiRenesas_OpenCamera_pack_WithTxt_L0.bin"
uCYModelEnd:

