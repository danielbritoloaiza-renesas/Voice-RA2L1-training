.section .rodata
.align 4

.global g_uSpeexDataBegin
.global g_uSpeexDataEnd

g_uSpeexDataBegin:
.incbin "../src/Model/DemoWaves8K_ENG.spxpack"
g_uSpeexDataEnd:
