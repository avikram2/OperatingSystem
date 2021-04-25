#ifndef _PAGING_H
#define _PAGING_H

#include "lib.h"
#include "processes.h"

//Directory and page size definitions
#define DIR_SIZE  				1024
#define TABLE_SIZE 				1024
#define BYTES_TO_ALIGN_TO		DIR_SIZE * 4

#define USER_DIRECTORY_IDX		32
#define USER_VIDEO_IDX			924
//Memory location Deffinitions
#define VIDEO_PHYS_ADDR 		0xB8000
#define KERNEL_PHYS_ADDR 		0x400000
#define PCB_PHYS_ADDR 		    0x800000
#define USER_ONE_PHYS_ADDR 		0x800000
#define USER_PHYS_ADDR_LEN 		0x400000

#define PHYSICAL_ADDR_SHIFT 	12
#define MAX_PROCESS_NUMBER 2
//Bit map definitions
#define P_MAP 					0x1		//Present bit map
#define R_MAP					0x2		//Read-Write bit map
#define S_MAP 					0x80	//Page Size bit map
#define U_MAP                   0x4     //user allowed
 
#define USER_MEM_START  0x8000000
#define USER_MEM_END  0x8400000
#ifndef ASM

uint32_t page_directory[DIR_SIZE] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
uint32_t first_page_table[DIR_SIZE] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
uint32_t kernel_page_table[DIR_SIZE] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
uint32_t pcb_page_table[DIR_SIZE] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
uint32_t user_page_table[DIR_SIZE] __attribute__((aligned(BYTES_TO_ALIGN_TO)));


// enable_paging() : function to enabe paging
void enable_paging();

// Formats a blank page table
void blank_table(uint32_t* table);

// ASM code to load page directory into CR3
void loadPageDirectory(unsigned int* addr);

// ASM code to load values into CR4 then CR0
void enablePaging();

void set_user_table(uint32_t process);


uint32_t set_user_video_mem(uint8_t** screen_start);

void remap_user(uint32_t table_idx, uint32_t virtual_addr);


#endif /* PAGING_H */

#endif
