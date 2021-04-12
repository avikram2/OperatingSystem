#ifndef _SYSCALL_LINKAGE_H
#define _SYSCALL_LINKAGE_H

#include "syscalls.h"


#ifndef ASM


//assembly linkage function system calls
extern void syscall_wrapper();

extern void ireturn();
#endif //ASM

#endif //syscall_linkage
