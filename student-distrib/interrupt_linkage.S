#define ASM     1


#include "interrupt_linkage.h"


.globl   keyboard_wrapper
.align   4
 
keyboard_wrapper:
    pushad
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call interrupt_keyboard_handler
    popad
    iret


.globl   rtc_wrapper
.align   4
rtc_wrapper:
    pushad
    cld /* C code following the sysV ABI requires DF to be clear on function entry */
    call interrupt_RTC_handler
    popad
    iret
