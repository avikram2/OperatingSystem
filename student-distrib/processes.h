#ifndef _PROCESS_H
#define _PROCESS_H

#include "lib.h"
#include "i8259.h"
#include "filesystem.h"
#include "paging.h"
#include "x86_desc.h"
#include "syscall_linkage.h"

#ifndef ASM
#define NUMBER_OF_FILE_DESCRIPTORS 8
#define NUMBER_OF_PROCESSES 2
#define PROCESS_ONE_PCB 0x7FE000
#define PROCESS_TWO_PCB 0x7FC000
#define KERNEL_STACK_ONE 0x800000
#define KERNEL_STACK_TWO 0x7FE000
#define FILE_LOCATION  0x08048000
#define BUFFER_SIZE_P 1024
#define MAGIC_NUMBER_ONE 0x7f
#define MAGIC_NUMBER_TWO 0x45
#define MAGIC_NUMBER_THREE 0x4c
#define MAGIC_NUMBER_FOUR 0x46
#define STARTING_POINT_LOCATION 24
#define STARTING_POINT_LENGTH 4

typedef struct fops {
    int32_t (*open)(const uint8_t* filename);
	int32_t (*close)(int32_t fd);
	int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
	int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
} fops_t;

typedef struct fd {
    fops_t* operations_table;
    uint32_t inode;
    uint32_t position;
    uint32_t flags;
} fd_t;

typedef struct pcb {
   fd_t file_descriptors[NUMBER_OF_FILE_DESCRIPTORS];
} pcb_t;



//halt system call
extern int32_t syscall_halt(uint8_t status);

//execute system call
extern int32_t syscall_execute(const uint8_t* command);

extern uint32_t check_file(const uint8_t* command, uint32_t* starting_address);

extern uint32_t load_file(const uint8_t* command);

extern void flush_tlb();

extern int32_t get_pid();

extern pcb_t** get_process();

#endif

#endif
