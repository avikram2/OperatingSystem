#include "filesystem.h"
#include "lib.h"

//global file system pointer
boot_block_t*	file_sys;

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
	
	int length = strlen((int8_t*)fname);
	
	
	//no input
	if(length == 0)
		return -1;
	
	int i;
	for(i = 0; i < NUM_DENTRIES; i++){
		
		int8_t* dentry_name = (int8_t*)file_sys->dentries[i].filename;
		
		//If current dentry name is equal to the file name, copy mem
		if (strncmp((int8_t*)fname, dentry_name, length) == 0){
			*dentry = file_sys->dentries[i];
			return 0;
		}
	}
	
	
	//file not found
	return -1;
}


/* read_dentry_by_index: sets a dentry block by idx
*
*
*
*/
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
	
	//Check if index is out of bounds
	if(index >= file_sys->num_dentries)
		return -1;
	
	*dentry = file_sys->dentries[index];
	return 0;
}





uint32_t read_inode_data_len(uint32_t inode_idx){
	
	if(inode_idx >= file_sys->num_dentries)
		return -1;
	
	inode_t* inode = (inode_t*)((uint32_t)file_sys + (inode_idx + 1) * SIZE_BLOCK);
	uint32_t length = inode->length;
	return length;
}





int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
	
	//Check if inode index is out of bounds
	if(inode > file_sys->num_inodes)
		return -1;
	
	//Check empty buffer?
	
	inode_t* inode_real = (inode_t*)((uint32_t)file_sys + (inode + 1) * SIZE_BLOCK);
	
	//change to read_inode_data_len
	uint32_t file_len = inode_real->length;
	
	if(offset > file_len)
		return -1;
	
	uint32_t num_blocks = file_sys->num_datablocks;
	
	uint32_t* data_block_start = (uint32_t*)((uint32_t)file_sys + (file_sys->num_inodes + 1) * SIZE_BLOCK);
	uint32_t data_block_idx = inode_real->data_blocks[0]+(offset / SIZE_BLOCK);
	
	if(data_block_idx > num_blocks)
		return -1;
	
	
	uint32_t* data_block_addr = (uint32_t*)((uint32_t)data_block_start + data_block_idx * SIZE_BLOCK);
	
	
	uint32_t num_bytes_read = 0;
	
	
	
	//Confirms that I receive entire string from file
	printf("%s", (char*)(data_block_addr + offset + num_bytes_read));
	
	
	
	//TO DO: UNCOMMENT AND FIX
	// while(num_bytes_read < length){
		
		// if(offset + num_bytes_read >= file_len){
			// return 0;
		// }
		// //possible mess up
		// // if((offset + num_bytes_read) % SIZE_BLOCK == 0){
			// // data_block_idx += 1;
			// // data_block_addr = (uint32_t*)((uint32_t)data_block_start + data_block_idx * SIZE_BLOCK);
		// // }
		
		
		// num_bytes_read += 1;
	// }
	
	return num_bytes_read;
}



