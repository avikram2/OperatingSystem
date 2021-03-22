#include "paging.h"
#include "lib.h"


 
unsigned int PAGE_SIZE = 1024;
 
//might need to  be static
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));



unsigned int KERNEL_PHYS_ADDR = 0x400000;


// format of Attributes: comments
// Attributes: "# - description (bit name)"
void enable_paging()
{
	unsigned int kernel_offset = KERNEL_PHYS_ADDR << 22;
	
	int i;
	for(i = 0; i < PAGE_SIZE; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		page_directory[i] = 0x00000002;
	}


	// initialize kernel table
	blank_table(kernel_page_table);
	unsigned int j;
	for(j = 0; j < PAGE_SIZE; j++)
	{
		// sets top bits to physical address
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		kernel_page_table[j] = (j * 0x1000 + kernel_offset) | 3; // attributes: supervisor level, read/write, present.
	}

	// Attributes: 1 - 4MB page (S), 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
	page_directory[1] = ((uint32_t)kernel_page_table) | 0x83;
	
	
	//TO DO: video memory
	blank_table(first_page_table);
	
	
	
	loadPageDirectory(page_directory);
	enablePaging();
}

void blank_table(uint32_t* table){
	int i;
	for(i = 0; i < 1024; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		table[i] = 0x00000002;
	}
	
}




//asm will need to be checked
void loadPageDirectory(uint32_t* addr) {
    asm volatile ("                 \n\
            movl    %0, %%cr3       \n\
            "
            :
            : "r" (addr)
    );
}

void enablePaging() {
    asm volatile ("                  	\n\
            movl	%%cr4, %%eax	  	\n\
			orl 	$0x00000010, %%eax 	\n\
			movl	%%eax, %%cr4		\n\
			xor		%%eax, %%eax		\n\
			movl	%%cr0, %%eax       	\n\
			orl 	$0x80000000, %%eax 	\n\
			movl 	%%eax, %%cr0       	\n\
            "
			:
            :
			: "%eax"
    );
}
