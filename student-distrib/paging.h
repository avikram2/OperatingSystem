#ifndef _PAGING_H
#define _PAGING_H

#include "lib.h"

//Directory and page size definitions
#define DIR_SIZE  				1024
#define TABLE_SIZE 				1024
#define BYTES_TO_ALIGN_TO		DIR_SIZE * 4

//Memory location Deffinitions
#define KERNEL_PHYS_ADDR 		0x400000
#define PCB_PHYS_ADDR 		0x800000
#define USER_ONE_PHYS_ADDR 		0xC00000
#define USER_TWO_PHYS_ADDR 		0x1000000
#define VIDEO_PHYS_ADDR 		0xB8
#define PHYSICAL_ADDR_SHIFT 	12
#define MAX_PROCESS_NUMBER 2
//Bit map definitions
#define P_MAP 					0x1		//Present bit map
#define R_MAP					0x2		//Read-Write bit map
#define S_MAP 					0x80	//Page Size bit map
#define U_MAP                                   0x4  //user allowed
 
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

#endif /* PAGING_H */

#endif