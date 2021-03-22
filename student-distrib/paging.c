#include "paging.h"
#include "lib.h"

static unsigned int DIR_SIZE, TABLE_SIZE = 1024;
unsigned int KERNEL_PHYS_ADDR = 0x400000;
unsigned int VIDEO_ADDR = 0xB8000;
unsigned int PHYSICAL_ADDR_SHIFT = 12;




/* enable_paging() : function to enabe paging
*INPUTS: NONE
*OUTPUTS: NONE
*DESCRIPTION: Created a page directory, and 2 page tables.
*			 Then loads page directory, and enables paging afterwards
*EFFECT: paging will be enabled, one 4MB page for 
*		 the kernel code, and one 4KB page for video memory 
*		 will be created

*   This code uses a lot of comments starting with "Attributes: "
*   format of Attributes comments
*   Attributes: "# - description (bit name)"
*/
void enable_paging()
{
	uint32_t kernel_offset = KERNEL_PHYS_ADDR << PHYSICAL_ADDR_SHIFT;
	uint32_t video_offset = VIDEO_ADDR << PHYSICAL_ADDR_SHIFT;
	unsigned int video_mem_idx = VIDEO_ADDR/0x1000;
	
	int i;
	for(i = 0; i < DIR_SIZE; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		page_directory[i] = 0x00000002;
	}


	// initialize kernel table
	blank_table(kernel_page_table);
	unsigned int j;
	for(j = 0; j < TABLE_SIZE; j++)
	{
		// sets top bits to physical address
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		kernel_page_table[j] = (j * 0x1000 + kernel_offset) | 3; // attributes: supervisor level, read/write, present.
	}

	// Attributes: 1 - 4MB page (S), 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[1] = ((unsigned int)kernel_page_table) | 0x83;
	
	
	//Set Video Memory
	blank_table(first_page_table);
	// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	first_page_table[video_mem_idx] = video_offset | 3;
	// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[0] = ((unsigned int)first_page_table) | 3;
	
	
	loadPageDirectory(page_directory);
	enablePaging();
}


// Formats a page blank table
// All indeces set to (supervisor, rw, not present)
void blank_table(uint32_t* table){
	int i;
	for(i = 0; i < TABLE_SIZE; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		table[i] = 0x00000002;
	}
	
}




// ASM code to load page directory into CR3
void loadPageDirectory(uint32_t* addr) {
    asm volatile ("                 \n\
            movl    %0, %%cr3       \n\
            "
            :
            : "r" (addr)
    );
}

// ASM code to load values into CR4 then CR0
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
