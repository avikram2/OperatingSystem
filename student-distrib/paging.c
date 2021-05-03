#include "paging.h"



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

	uint32_t kernel_offset = (KERNEL_PHYS_ADDR >> PHYSICAL_ADDR_SHIFT) << PHYSICAL_ADDR_SHIFT;
	uint32_t video_offset = (VIDEO_PHYS_ADDR >> PHYSICAL_ADDR_SHIFT) << PHYSICAL_ADDR_SHIFT;

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
	page_directory[1] = ((unsigned int)KERNEL_PHYS_ADDR) | S_MAP | R_MAP | P_MAP;


	set_user_table(0);


	//Set Video Memory
	blank_table(first_page_table);

	// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	first_page_table[VIDEO_PHYS_ADDR >> PHYSICAL_ADDR_SHIFT] = video_offset | R_MAP | P_MAP;

	// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 1 - present (P)
	page_directory[0] = ((unsigned int)first_page_table) | R_MAP | P_MAP;


	loadPageDirectory(page_directory);
	enablePaging();
}

/* set_user_video_mem(): function to map to user video mem
*INPUTS: uint8_t** screen_start maps the address of the screen display start address to a page value
*OUTPUTS: -1 for error
*DESCRIPTION: Sets up accessible video memory for the user space
*/
uint32_t set_user_video_mem(uint8_t** screen_start)
{
	if(screen_start == 0 || *screen_start == 0)
	{
		return -1;
	}
	if(*screen_start < (uint8_t*)USER_MEM_START || *screen_start >= (uint8_t*)USER_MEM_END)
	{
		return -1;
	}

	uint32_t video_offset = VIDEO_PHYS_ADDR << PHYSICAL_ADDR_SHIFT;

	int j = (((uint32_t)*screen_start) - USER_MEM_START) >> PHYSICAL_ADDR_SHIFT;
	user_page_table[j] = (video_offset) | U_MAP | R_MAP | P_MAP;
	return 0;
}

/* set_user_table(): setup the user table
*INPUTS: uint32_t process, the current running process
*OUTPUTS: NONE
*DESCRIPTION: Sets the user table to present, writeable, and user accessible
*/
void set_user_table(uint32_t process)
{
	if(process < 0 || process >= NUMBER_OF_PROCESSES)
	{
		return;
	}
	uint32_t user_pos = USER_ONE_PHYS_ADDR;
	user_pos = user_pos + USER_PHYS_ADDR_LEN * process;
	uint32_t user_offset = (user_pos >> PHYSICAL_ADDR_SHIFT) << PHYSICAL_ADDR_SHIFT;

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

	page_directory[USER_DIRECTORY_IDX] = ((unsigned int)user_pos) | S_MAP | U_MAP | R_MAP | P_MAP;

	loadPageDirectory(page_directory);
}

/* blank_table() :
*INPUTS: uint32_t* table, pointer to table to be blanked
*OUTPUTS: NONE
*DESCRIPTION: Helper function to blank a table, and set write permissions
*/
void blank_table(uint32_t* table){
	int i;
	for(i = 0; i < TABLE_SIZE; i++)
	{
		// Attributes: 0 -  kernel-mode (U - supervisor mode), 1 - read/write (R), 0 - not present (P)
		table[i] = R_MAP;
	}

}

/* remap_user() :
*INPUTS: uint32_t table_idx, idx of table in directory
*		 uint32_t virtual_addr, new addr to put in idx
*OUTPUTS: NONE
*DESCRIPTION: Maps user table to a new idx
*/
void remap_user(uint32_t table_idx, uint32_t virtual_addr){

	//allign virtual address
	uint32_t virtual_offset = (virtual_addr >> PHYSICAL_ADDR_SHIFT) << PHYSICAL_ADDR_SHIFT;

	//Set user page
	user_page_table[0] = virtual_offset | U_MAP | R_MAP | P_MAP;
	page_directory[table_idx] = ((unsigned int)user_page_table) | U_MAP | R_MAP | P_MAP;

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
