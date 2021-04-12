#define ASM 1

#define MAX_CALL_NUMBER 9
#include "syscall_linkage.h"


.globl   syscall_wrapper, ireturn
.align   4
 # linkage function between assembly and c system call functions
syscall_wrapper:
    #save registers
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    #check for valid call number
    cmpl $0,%eax
    jl BAD_CALL_NUMBER
    cmpl MAX_CALL_NUMBER,%eax
    jg BAD_CALL_NUMBER
    movl SYS_CALL_JUMP_TABLE(,%eax,4),%eax
    call *%eax
    #restore registers
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    iret

BAD_CALL_NUMBER:
    movl -1,%eax
    iret

ireturn:
	//user ds
	pushl $0x2b
	//user stack location
	pushl $0x8400000 
	pushfl
    //allow interrupts
popl  %eax
        orl   $0x200,%eax
        pushl %eax
	//user cs
	pushl $0x23
	//passed in program start location
	pushl %eax  
	iret

SYS_CALL_JUMP_TABLE:
.long syscall_halt,syscall_execute,syscall_read,syscall_write,syscall_open,syscall_close,syscall_getargs,syscall_vidmap,syscall_set_handler,syscall_sigreturn

