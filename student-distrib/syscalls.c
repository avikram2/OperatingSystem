
#include "syscalls.h"


// execute:
// executes program
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_execute(const uint8_t* command){
    return -1;
}

// halt:
// halts program
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_halt(const uint8_t status){
    return -1;
}

// read:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes){
    return -1;
}

// write:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}

// open:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_open(const uint8_t* filename){
    return -1;
}

// close:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_close(const uint32_t fd){
    return -1;
}

// getargs:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_getargs(uint8_t* buf, int32_t nbytes){
    return -1;
}

// vidmap:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_vidmap(uint8_t** screen_start){
    return -1;
}

// set_handler:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_set_handler(int32_t signum, void* handle_address){
    return -1;
}
// sigreturn:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_sigreturn(void){
    return -1;
}



