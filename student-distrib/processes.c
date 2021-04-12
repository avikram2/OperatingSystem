
#include "processes.h"


// execute:
// executes program
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_execute(const uint8_t* command){
    uint32_t starting_address;
    int i;

	int out;
    if(command == 0)
    {
        return -1;
    }
    //check if its an executable
    if(!check_file(command,&starting_address))
    {
	return -1;
    }

    current_process++;
    //only two processes allowed at a time
    if(current_process < 0 || current_process > 1)
    {
	current_process--;
        return -1;
    }

    set_user_table(current_process);
    
    if(!load_file(command))
    {
        current_process--;
        set_user_table(current_process);
    }

    //set fds to unused
    for(i = 0;i<NUMBER_OF_FILE_DESCRIPTORS;i++)
    {
        processes[current_process]->file_descriptors[i].flags = 0;
    }

    //need to set stdin and stdout

    tss.esp0 = kernel_stacks[current_process];
    
    //push iret context
    asm volatile("pushl 0x23 \n\t"
		"pushl %%esp \n\t"
		"pushf \n\t"
		"pushl 0x001b   \n\t"
		"pushl %1    \n\t"
		"iret \n\t"
		"movl %%eax,%0 \n\t"
		: "=r" (out)
                : "r" (starting_address));

    //get halt results
    return 1;
}

// halt:
// halts program
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_halt(const uint8_t status){
    return -1;
}

uint32_t check_file(const uint8_t* command,uint32_t* starting_address)
{
	dentry_t dentry;
	int read;
        uint8_t buf[28];
	if(read_dentry_by_name(command, &dentry) == -1)
		return 0;
	read = read_data(dentry.inode_idx, 0, buf, 27);
	if(read < 28)
	{
	    return 0;
        }
	//check for executable magic number
	if(buf[0] != 0x7f || buf[1] != 0x45 || buf[2] != 0x4c || buf[3] != 0x46)
	{
		return 0;
	}
        //get starting address
        *starting_address = 0;
        *starting_address = (buf[24] << 24) | (buf[25] << 16) | (buf[26] << 8) | (buf[27] << 0);
	return 1;
}

uint32_t load_file(const uint8_t* command)
{
	dentry_t dentry;
	int read;
        int not_done = 1;
        int count = 0;
        int position = 0;
        uint8_t buf[BUFFER_SIZE];


    if(command == 0)
    {
        return -1;
    }
	if(read_dentry_by_name(command, &dentry) == -1)
		return 0;
        
        while(not_done)
        {	
		read = read_data(dentry.inode_idx, position, buf, BUFFER_SIZE);
                if(read <= 0)
                {
                    not_done = 0;
                }
                for(count = 0;count < read;count++,position++)
		{
			*(uint8_t*)(FILE_LOCATION + position) = buf[count];
		}
        }
     return 1;
}


