#include "filesystem.h"
#include "lib.h"

//global file system pointer
boot_block_t*	file_sys;


/* read_dentry_by_name: sets a dentry block by searching for filename
*  inputs: fname - name of requested file
*			dentry - the dentry as a dentry struct
*  outputs: 0 for success, -1 for failure
*
*/
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
*  inputs: index - dentry index
*			dentry - the dentry as a dentry struct
*  outputs: 0 for success, -1 for failure
*
*/
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
	
	//Check if index is out of bounds
	if(index >= file_sys->num_dentries)
		return -1;
	
	*dentry = file_sys->dentries[index];
	return 0;
}




/* read_inode_data_len: finds length of inode data
*  inputs: inode_idx - index of inode in blocks
*  outputs: length on success, -1 for failure
*
*/
uint32_t read_inode_data_len(uint32_t inode_idx){
	
	if(inode_idx >= file_sys->num_inodes)
		return -1;
	
	inode_t* inode = (inode_t*)((uint32_t)file_sys + (inode_idx + 1) * SIZE_BLOCK);
	uint32_t length = inode->length;
	return length;
}


/* read_data: read data from a file
*  inputs: inode - index of inode in blocks
*			offset - offset of requested bits in file
*			buf - buffer to pass character stream into
*			length - number of requested bits to read
*  outputs: number of bytes read on success, -1 for failure,
*			0 on reaching file end
*
*/
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
	
	//Check if inode index is out of bounds
	if(inode > file_sys->num_inodes)
		return -1;
	
	inode_t* inode_real = (inode_t*)((uint32_t)file_sys + (inode + 1) * SIZE_BLOCK);
	
	//change to read_inode_data_len
	uint32_t file_len = read_inode_data_len(inode);
	
	if(offset > file_len)
		return -1;
	
	uint32_t num_blocks = file_sys->num_datablocks;
	uint32_t* data_block_start = (uint32_t*)((uint32_t)file_sys + (file_sys->num_inodes + 1) * SIZE_BLOCK);
	
	//get idx of db idx value, and db idx value
	int block_idx = offset/SIZE_BLOCK;
	uint32_t data_block_idx = inode_real->data_blocks[block_idx];
	
	if(data_block_idx > num_blocks)
		return -1;

	//get address of data_block
	uint32_t* data_block_addr = (uint32_t*)((uint32_t)data_block_start + data_block_idx * SIZE_BLOCK);
	
	
	//set variables used in iterations
	int num_bytes_read = 0;
	uint32_t count_null_char = 0;
	char* tracker;
	while (num_bytes_read < length) {
		
		//Check for file end
		if (offset + num_bytes_read > file_len)
			return 0;
			
		//Check for block end
		if ((offset + num_bytes_read) % SIZE_BLOCK == 0) {
			data_block_idx = *((uint32_t*)inode_real + (block_idx + 1));
			data_block_addr = (uint32_t*)((uint32_t)data_block_start + data_block_idx * SIZE_BLOCK);
			block_idx++;
		}
		
		tracker = (char*)data_block_addr + (offset + num_bytes_read) % SIZE_BLOCK;
		
		//Disclude null char 
		if(*tracker == '\0')
			count_null_char++;
		else
			buf[num_bytes_read - count_null_char] = *tracker; 
		num_bytes_read++;
	}
	
	return num_bytes_read;
}

/* file_read: read data from a file
*  inputs: fname - file name
*			buf - buffer to pass character stream into
*			nbytes - number of requested bits to read
*  outputs: number of bytes read on success, -1 for failure,
*			0 on reaching file end
*
*/
int32_t file_read(uint8_t* fname, uint8_t* buf, int32_t nbytes){

	dentry_t* dentry;
	if(read_dentry_by_name(fname, dentry) == -1)
		return -1;
	return read_data(dentry->inode_idx, 0, buf, nbytes);
	
}

/* directory_read: read files from directory
*  inputs: buf - buffer to pass character stream into
*			nbytes - number of requested bits to read
*  outputs: number of bytes read on success, -1 for failure,
*			0 on reaching file end
*
*/
int32_t directory_read(uint8_t* fname, uint8_t* buf, int32_t nbytes){
    
	dentry_t* dentry;
	uint32_t cnt, idx = 0;

	while(-1 != read_dentry_by_index(idx, dentry)){
		
		printf("%s   size:%d\n", (char*)dentry->filename, read_inode_data_len(dentry->inode_idx));
		idx++;
		
	}
	if(idx != file_sys->num_dentries)
		return -1;

	return 0;

    // while (-1 != (cnt = file_read(fname, buf, FS_BUF_LENGTH-1))) {
        // if (-1 == cnt) {
	        // printf("directory entry read failed\n");
	        // return cnt;
	    // }
		// printf("%s\n", buf);
		// idx++;
		// if(read_dentry_by_index(idx, dentry) == -1)
			// return -1;
		
    // }
	
}

//file_open: yet to do anything
int32_t file_open(const uint8_t* fname){
	return 0;
}

//file_close: yet to do anything
int32_t file_close(int32_t fd){
	return 0;
}

//file_write: not supported
int32_t file_write(const uint8_t* fname, const uint8_t* buf, int32_t nbytes){
	return -1;
}

//directory_open: yet to do anything
int32_t directory_open(const uint8_t* fname){
	return 0;
}

//directory_close: yet to do anything
int32_t directory_close(int32_t fd){
	return 0;
}

//directory_write: not supported
int32_t directory_write(const uint8_t* fname, const uint8_t* buf, int32_t nbytes){
	return -1;
}
