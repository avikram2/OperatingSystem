#include "paging.h"
#include "lib.h"


 
 
//might need to  be static
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));



unsigned int KERNEL_PHYS_ADDR = 0x400000;



void enable_paging()
{
	unsigned int kernel_offset = KERNEL_PHYS_ADDR << 12;
	
	int i;
	for(i = 0; i < 1024; i++)
	{
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	unsigned int j;
	 
	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(j = 0; j < 1024; j++)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[j] = (j * 0x1000 + kernel_offset) | 3; // attributes: supervisor level, read/write, present.
	}

	page_directory[1] = ((unsigned int)first_page_table) | 3;
	
	loadPageDirectory(page_directory);
	enablePaging();
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
    asm volatile ("                  \n\
            movl	%%cr0, %%eax       \n\
			orl 	$0x80000000, %%eax     \n\
			movl 	%%eax, %%cr0       \n\
            "
			:
            :
			: "%eax"
    );
}
