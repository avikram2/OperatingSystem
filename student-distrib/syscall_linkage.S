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
    cmpl $MAX_CALL_NUMBER,%eax
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
	movw $0x2b, %ax //user_ds value
	movw %ax, %ds
	movl $0x8300000,%ecx
	//movw %ax, %es 
	//movw %ax, %fs
	//movw %ax, %gs 
	//user ds
    movl $0x002b, %eax
	pushl %eax //push ss (user_ds)
	//user stack location
	pushl $0x8300000
	pushfl
    popl  %ebx
    orl   $0x200,%ebx
    pushl %ebx

	//user cs
	pushl $0x23 // USER_CS VALUE
	//passed in program start location
	pushl %edx
	iret

SYS_CALL_JUMP_TABLE:
.long syscall_halt,syscall_execute,syscall_read,syscall_write,syscall_open,syscall_close,syscall_getargs,syscall_vidmap,syscall_set_handler,syscall_sigreturn

