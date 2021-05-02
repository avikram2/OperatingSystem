#define ASM 1


#include "interrupt_linkage.h"

#define IF_BIT_MAP 0x200
#define FLAGS_OFFSET 12

.globl   keyboard_wrapper
.align   4
 # linkage function between assembly and c interrupt handler, unsued for checkpoint 1
keyboard_wrapper:
    cli
    pushal
    pushfl
    //pushl %ds
    //pushl %fs
    //pushl %es
    pushl %eax
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx 
//handler arguments
    pushl %esp
    pushl $keyboard_return
    call interrupt_keyboard_handler
    popl %edx
    popl %edx

keyboard_return:
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    popl %eax
    //popl %es
    //popl %fs
    //popl %ds
    popfl
    popal
    sti
    iret


.globl   rtc_wrapper
.align   4
rtc_wrapper:
    cli
    pushal
    pushfl
    //pushl %ds
    //pushl %fs
    //pushl %es
    pushl %eax
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx

//handler arguments
    pushl %esp
    pushl $rtc_return
    call rtc_handler
    popl %edx
    popl %edx

rtc_return:
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    popl %eax
    //popl %es
    //popl %fs
    //popl %ds
    popfl
    popal
    sti
    iret

