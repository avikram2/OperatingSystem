#define ASM 1


#include "interrupt_linkage.h"


.globl   keyboard_wrapper
.align   4
 # linkage function between assembly and c interrupt handler, unsued for checkpoint 1
keyboard_wrapper:
    pushal
    call interrupt_keyboard_handler
    popal
    iret


