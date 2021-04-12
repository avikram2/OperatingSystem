#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "lib.h"
#include "i8259.h"
#include "processes.h"
#include "filesystem.h"

#include "rtc_driver.h"


#define ACTIVE_FLAG 1

#define NUMBER_OF_FILE_DESCRIPTORS 8




#ifndef ASM


void * rtc_ops[4] = {rtc_open, rtc_close, rtc_read, rtc_write};

void * dir_ops[4] = {directory_open, directory_close, directory_read, directory_write};

void * file_ops[4] = {file_open, file_close, file_read, file_close};

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
