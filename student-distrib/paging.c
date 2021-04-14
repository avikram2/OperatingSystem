#include "paging.h"



uint32_t process_user_addresses[MAX_PROCESS_NUMBER] = {USER_ONE_PHYS_ADDR,USER_TWO_PHYS_ADDR};
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
	//uint32_t pcb_offset = PCB_PHYS_ADDR << PHYSICAL_ADDR_SHIFT;
	uint32_t video_offset = VIDEO_PHYS_ADDR << PHYSICAL_ADDR_SHIFT;
	
	int i;
	for(i = 0; i < DIR_SIZE; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		page_directory[i] = R_MAP;
	}


	// initialize kernel table
	blank_table(kernel_page_table);
	unsigned int j;
	for(j = 0; j < TABLE_SIZE; j++)
	{
		// sets top bits to physical address
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		kernel_page_table[j] = (j * BYTES_TO_ALIGN_TO + kernel_offset) | R_MAP | P_MAP; // attributes: supervisor level, read/write, present.
	}

	// Attributes: 1 - 4MB page (S), 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[1] = ((unsigned int)kernel_page_table) | S_MAP | R_MAP | P_MAP;
	
	/*// initialize pcb table
	blank_table(pcb_page_table);
	for(j = 0; j < TABLE_SIZE; j++)
	{
		// sets top bits to physical address
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		pcb_page_table[j] = (j * BYTES_TO_ALIGN_TO + pcb_offset) | R_MAP | P_MAP; // attributes: supervisor level, read/write, present.
	}

	// Attributes: 1 - 4MB page (S), 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[2] = ((unsigned int)pcb_page_table) | S_MAP | R_MAP | P_MAP;
	*/
	set_user_table(0);
	
	
	//Set Video Memory
	blank_table(first_page_table);
	// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	first_page_table[VIDEO_PHYS_ADDR] = video_offset | R_MAP | P_MAP;
	// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[0] = ((unsigned int)first_page_table) | R_MAP | P_MAP;
	
	
	loadPageDirectory(page_directory);
	enablePaging();
}

void set_user_table(uint32_t process)
{
	if(process < 0 || process > 1)
	{
		return;
	}

	uint32_t user_offset = process_user_addresses[process] << PHYSICAL_ADDR_SHIFT;
	
	// initialize user table
	blank_table(user_page_table);
	unsigned int j;
	for(j = 0; j < TABLE_SIZE; j++)
	{
		// sets top bits to physical address
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		user_page_table[j] = (j * BYTES_TO_ALIGN_TO + user_offset) | U_MAP | R_MAP | P_MAP; // attributes: supervisor level, read/write, present.
	}

	// Attributes: 1 - 4MB page (S), 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[32] = ((unsigned int)user_page_table) | U_MAP | S_MAP | R_MAP | P_MAP;

	loadPageDirectory(page_directory);
}

// Formats a page blank table
// All indeces set to (supervisor, rw, not present)
void blank_table(uint32_t* table){
	int i;
	for(i = 0; i < TABLE_SIZE; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		table[i] = R_MAP;
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
