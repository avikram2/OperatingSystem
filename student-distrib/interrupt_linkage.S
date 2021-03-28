#define ASM 1


#include "interrupt_linkage.h"


.globl   keyboard_wrapper
.align   4
 # linkage function between assembly and c interrupt handler, unsued for checkpoint 1
keyboard_wrapper:
    pushfl
    pushal
    call interrupt_keyboard_handler
    popal
    popfl
    iret


.globl   rtc_wrapper
.align   4
rtc_wrapper:
    pushfl
    pushal
    call rtc_handler
    popal
    popfl
    iret   