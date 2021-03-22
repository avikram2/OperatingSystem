#ifndef PAGING_H
#define PAGING_H

#include "lib.h"
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));


// enable_paging() : function to enabe paging
void enable_paging();

// Formats a blank page table
void blank_table(uint32_t* table);

// ASM code to load page directory into CR3
void loadPageDirectory(unsigned int* addr);

// ASM code to load values into CR4 then CR0
void enablePaging();

#endif /* PAGING_H */
