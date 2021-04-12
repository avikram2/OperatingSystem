
#include "processes.h"

int current_process = -1;
uint32_t kernel_stacks[NUMBER_OF_PROCESSES] = {KERNEL_STACK_ONE,KERNEL_STACK_TWO};
pcb_t* processes[NUMBER_OF_PROCESSES] = {(pcb_t*)PROCESS_ONE_PCB,(pcb_t*)PROCESS_TWO_PCB};


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
    tss.ss0 = KERNEL_DS;


    //push iret context
    asm volatile("movl %1,%%edx  \n\t"
		: "=r" (out)
                : "r" (starting_address));
    //get halt results
	ireturn();
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
        uint8_t buf[BUFFER_SIZE];
        uint8_t buf2[4];
	if(read_dentry_by_name(command, &dentry) == -1)
		return 0;
	read = read_data(dentry.inode_idx, 0, buf, BUFFER_SIZE);

	//check for executable magic number
	if(buf[0] != MAGIC_NUMBER_ONE || buf[1] != MAGIC_NUMBER_TWO || buf[2] != MAGIC_NUMBER_THREE || buf[3] != MAGIC_NUMBER_FOUR)
	{
		return 0;
	}
        //get starting address
	read_data( dentry.inode_idx, STARTING_POINT_LOCATION, buf2, STARTING_POINT_LENGTH);
        *starting_address = *((uint32_t*)buf2);

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

//get_pid
//function to return the current process id
int32_t get_pid(){
    return current_process;
}

//get_process
//function to return processes id
pcb_t** get_process(){
    return processes;
}
