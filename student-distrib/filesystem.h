#ifndef _FILE_SYS_H
#define _FILE_SYS_H

#include "lib.h"





//inode definitions
#define MAX_BLOCKS		1023


//reserved space dfinitions
#define RESERVED_BOOT_BLOCK	52
#define RESERVED_DENTRY		24

//boot block efinitions
#define NUM_DENTRIES 	63


//dentry definitions
#define MAX_NAME_LENGTH 32
#define SIZE_DENTRY		0x40

//block definitions
#define MAX_DATA_BLOCKS 1023
#define SIZE_BLOCK 		0x1000


//file type difinitions
#define FILE_USER 			0
#define	FILE_DIRECTORY		1
#define FILE_REGULAR		2

typedef struct dentry_t{
	uint8_t	 filename[MAX_NAME_LENGTH];
	uint32_t type;
	uint32_t inode_idx;
	uint8_t  reserved_space[RESERVED_DENTRY];
	
}dentry_t;


typedef struct boot_block_t{
	
	uint32_t num_dentries;
	uint32_t num_inodes;
	uint32_t num_datablocks;
	uint8_t	 reserved_space[RESERVED_BOOT_BLOCK];
	dentry_t dentries[NUM_DENTRIES];
	
	
}boot_block_t;

typedef struct inode_t{
	uint32_t length;
	uint32_t data_blocks[MAX_DATA_BLOCKS];
}inode_t;


int32_t read_dentry_by_name (const uint8 t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);


#endif
