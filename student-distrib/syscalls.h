#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "lib.h"
#include "i8259.h"
#include "processes.h"

#ifndef ASM


//read system call
extern int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes);

//write system call
extern int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes);

//open system call
extern int32_t syscall_open(const uint8_t* filename);

//close system call
extern int32_t syscall_close(const uint32_t fd);

//getargs system call
extern int32_t syscall_getargs(uint8_t* buf, int32_t nbytes);

//vidmap system call
extern int32_t syscall_vidmap(uint8_t** screen_start);

//set_handler system call
extern int32_t syscall_set_handler(int32_t signum, void* handle_address);

//sigreturn system call
extern int32_t syscall_sigreturn(void);


#endif

#endif
