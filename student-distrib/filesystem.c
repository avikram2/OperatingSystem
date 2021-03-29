#include "filesystem.h"
#include "lib.h"

//global file system pointer
boot_block_t*	file_sys;

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
	
	int length = strlen((int8_t*)fname);
	
	
	//no input
	if(length == 0)
		return -1;
	
	int i;
	for(i = 0; i < NUM_DENTRIES; i++){
		
		int8_t* dentry_name = (int8_t*)file_sys->dentries[i].filename;
		
		//If current dentry name is equal to the file name, copy mem
		if (strncmp((int8_t*)fname, dentry_name, length) == 0){
			memcpy(dentry, &file_sys->dentries[i], length);
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
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
	
	//Check if index is out of bounds
	if(index >= file_sys->num_dentries)
		return -1;
	
	memcpy(dentry, &file_sys->dentries[index], SIZE_DENTRY);
	return 0;
}





uint32_t read_inode_data_len (uint32_t inode_idx){
	
	if(inode_idx >= file_sys->num_dentries)
		return -1;
	
	inode_t* inode = (inode_t*)((uint32_t)file_sys + (inode_idx + 1) * SIZE_BLOCK);
	uint32_t length = inode->length;
	return length;
}





int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
	
	
	
	return -1;
}
