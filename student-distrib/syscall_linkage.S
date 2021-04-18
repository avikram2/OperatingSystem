#define ASM 1

#define MAX_CALL_NUMBER 10
#define MIN_CALL_NUMBER 1
#define USER_DS_VALUE 0x2b
#define USER_CS_VALUE 0x23
#define USER_STACK 0x8300000
#define IF_BIT 0x200

#define IRETURN_STARTING_ADDRESS_OFFSET 4
#define IRETURN_ESP_POINTER_OFFSET 8
#define IRETURN_EBP_POINTER_OFFSET 12
#include "syscall_linkage.h"


.globl   syscall_wrapper, ireturn, ireturn_return
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
    cmpl $MIN_CALL_NUMBER,%eax
    jl BAD_CALL_NUMBER
    cmpl $MAX_CALL_NUMBER,%eax
    jg BAD_CALL_NUMBER
    subl $1,%eax
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
    movl $-1,%eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    iret

ireturn:
	movl IRETURN_EBP_POINTER_OFFSET(%esp),%edx
	movl %ebp,(%edx)
	movl IRETURN_ESP_POINTER_OFFSET(%esp),%edx
	movl %esp,(%edx)
	movl IRETURN_STARTING_ADDRESS_OFFSET(%esp),%edx
	movw $USER_DS_VALUE, %ax //user_ds value
	movw %ax, %ds
	//user ds
    	movl $USER_DS_VALUE, %eax
	pushl %eax //push ss (user_ds)
	//user stack location
	pushl $USER_STACK
	pushfl
	//set IF to 1
    	popl  %ebx
    	orl   $IF_BIT,%ebx
    	pushl %ebx

	//user cs
	pushl $USER_CS_VALUE // USER_CS VALUE
	//passed in program start location
	pushl %edx
	iret
ireturn_return:
	ret

SYS_CALL_JUMP_TABLE:
.long syscall_halt,syscall_execute,syscall_read,syscall_write,syscall_open,syscall_close,syscall_getargs,syscall_vidmap,syscall_set_handler,syscall_sigreturn

