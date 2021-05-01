#ifndef _PROCESS_H
#define _PROCESS_H

#include "lib.h"
#include "i8259.h"
#include "filesystem.h"
#include "paging.h"
#include "x86_desc.h"
#include "syscall_linkage.h"
#include "terminal.h"

#ifndef ASM
#define NUMBER_OF_FILE_DESCRIPTORS 8
#define NUMBER_OF_PROCESSES 4
#define PROCESS_ONE_PCB 0x7FE000
#define PROCESS_TWO_PCB 0x7FC000
#define PROCESS_THREE_PCB 0x7FA000
#define PROCESS_FOUR_PCB 0x7F8000
#define KERNEL_STACK_START 0x800000
#define PROC_KERNEL_LEN 0x2000
#define FILE_LOCATION  0x08048000
#define BUFFER_SIZE_P 1024
#define MAGIC_NUMBER_ONE 0x7f
#define MAGIC_NUMBER_TWO 0x45
#define MAGIC_NUMBER_THREE 0x4c
#define MAGIC_NUMBER_FOUR 0x46
#define STARTING_POINT_LOCATION 24
#define STARTING_POINT_LENGTH 4
#define ARGS_BUFFER_SIZE 128
typedef struct fops {
    int32_t (*open)(const uint8_t* filename,int32_t fd);
	int32_t (*close)(int32_t fd);
	int32_t (*read)(int32_t fd, uint8_t* buf, int32_t nbytes);
	int32_t (*write)(int32_t fd, const uint8_t* buf, int32_t nbytes);
} fops_t;

typedef struct fd {
    fops_t* fops_table;
    //void * fops_table;
    uint32_t inode;
    uint32_t position;
    uint32_t flags;
} fd_t;

typedef struct pcb {
   fd_t file_descriptors[NUMBER_OF_FILE_DESCRIPTORS];
   uint32_t parent_esp;
   uint32_t parent_ebp;
   uint8_t args[ARGS_BUFFER_SIZE];
   uint32_t args_len;
   uint32_t process_term;
} pcb_t;



//halt system call
extern int32_t syscall_halt(uint8_t status);

//execute system call
extern int32_t syscall_execute(const uint8_t* command);

extern uint32_t check_file(const uint8_t* command, uint32_t* starting_address, uint32_t* inode);

extern uint32_t load_file(uint32_t inode);

extern void flush_tlb();

extern int32_t get_pid();

extern pcb_t** get_process();
//for stdint/out, doesn't do anything
extern int32_t std_open(const uint8_t* filename, int32_t fd);

//for stdint/out, doesn't do anything
extern int32_t std_close(int32_t fd);

//for stdint/out, doesn't do anything
extern int32_t std_read(int32_t fd, uint8_t* buf, int32_t nbytes);

//for stdint/out, doesn't do anything
extern int32_t std_write(int32_t fd, const uint8_t* buf, int32_t nbytes);

//halts a program from an exception
extern void exception_halt();

extern void store_args(const uint8_t* command);

extern void get_command(uint8_t* filename, const uint8_t* command);

//program for switching to a new process
extern void switch_process(int32_t pid);
#endif

#endif
