#define ASM 1


#include "interrupt_linkage.h"


.globl   keyboard_wrapper
.align   4
 
keyboard_wrapper:
    pushal
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call interrupt_keyboard_handler
    popal
    iret


