
#include "processes.h"
int current_process = -1;

pcb_t* processes[NUMBER_OF_PROCESSES] = {(pcb_t*)PROCESS_ONE_PCB,(pcb_t*)PROCESS_TWO_PCB,(pcb_t*)PROCESS_THREE_PCB,(pcb_t*)PROCESS_FOUR_PCB,(pcb_t*)PROCESS_FIVE_PCB,(pcb_t*)PROCESS_SIX_PCB};

static fops_t stdin_ops = {std_open,std_close,terminal_read,std_write};
static fops_t stdout_ops = {std_open,std_close,std_read,terminal_write};

static int exception_halt_triggered = 0;
// execute:
// executes program
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_execute(const uint8_t* command){

	//cli();
    uint32_t starting_address;
    uint32_t inode;
    const uint8_t filename[4] = "";
    int i;
    if(command == 0)
    {
	//sti();
        return -1;
    }
    //check if its an executable
    if(!check_file(command,&starting_address,&inode))
    {
	//sti();
	return -1;
    }

    if(terminal_info.terminal_pid_lengths[terminal_info.current_terminal] >= MAX_TERMINAL_PROCESSES)
    {
	//sti();
        return -1;
    }

    int old_process = current_process;
    int found = 0;
    int empty = 0;
    for(i = 0;i<NUMBER_OF_PROCESSES;i++)
    {
        if(processes_status[i] == 0)
 	{
		if(!found)
		{
			current_process = i;
  			found = 1;
		}
		empty++;
	}
    }
    int remaining_terminals = 0;
    for(i=0;i<TERMINAL_NUMBER;i++)
    {
        if(terminal_info.active_terminals[i] == 0)
	{
		remaining_terminals++;
	}
    }
    if(remaining_terminals >= empty)
    {
        current_process = old_process;
	//sti();
	return -1;
    }
    if(!found)
    {
	//no empty processes
	//sti();
        return -1;
    }
 


    store_args(command);

    set_user_table(current_process);

    flush_tlb();

    if(!load_file(inode))
    {
        current_process = old_process;
        set_user_table(current_process);

    flush_tlb();
	//sti();
	return -1;
    }

    terminal_info.terminal_pid_lengths[terminal_info.current_terminal]++;
    terminal_info.terminal_pids[terminal_info.current_terminal][terminal_info.terminal_pid_lengths[terminal_info.current_terminal]-1] = current_process;
    processes_status[current_process] = 1;
    //set fds to unused
    for(i = 0;i<NUMBER_OF_FILE_DESCRIPTORS;i++)
    {
        processes[current_process]->file_descriptors[i].flags = 0;
    }

    processes[current_process]->file_descriptors[0].flags = ACTIVE_FLAG;
    processes[current_process]->file_descriptors[0].fops_table = &stdin_ops;


    processes[current_process]->file_descriptors[1].flags = ACTIVE_FLAG;
    processes[current_process]->file_descriptors[1].fops_table = &stdout_ops;
    processes[current_process]->file_descriptors[1].fops_table->open(filename, 1);

    //need to set stdin and stdout
    uint32_t stack = KERNEL_STACK_START;
    stack = stack - PROC_KERNEL_LEN * current_process;
    tss.esp0 = stack - 4;
    tss.ss0 = KERNEL_DS;

    //set terminal data
    set_process(terminal_info.current_terminal);
    processes[current_process]->process_term = terminal_info.current_terminal;

    int ret = ireturn(starting_address,&(processes[current_process]->parent_esp),&(processes[current_process]->parent_ebp));
   
	//sti();
 return ret;
}

// halt:
// halts program
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_halt(const uint8_t status){
	//cli();
    uint32_t ret_status = (uint32_t)status;
    if(exception_halt_triggered)
    {
        ret_status = 256;
    }
    exception_halt_triggered = 0;
    //set fds to unused
    if(current_process >= NUMBER_OF_PROCESSES || current_process<0)
    {
	//sti();
        return -1;
    }
    if(terminal_info.terminal_pid_lengths[terminal_info.current_terminal] <= 0)
    {
	//sti();
        return -1;
    }
    int i;
    for(i = 0;i<NUMBER_OF_FILE_DESCRIPTORS;i++)
    {
        processes[current_process]->file_descriptors[i].flags = 0;
	if(processes[current_process]->file_descriptors[i].flags == ACTIVE_FLAG)
	{
    		processes[current_process]->file_descriptors[1].fops_table->close(i);
	}
    }
    int old_process = current_process;
    terminal_info.terminal_pid_lengths[terminal_info.current_terminal]--;
    if(terminal_info.terminal_pid_lengths[terminal_info.current_terminal] == 0)
    {
	current_process = -1;
    }else{
    	current_process = terminal_info.terminal_pids[terminal_info.current_terminal][terminal_info.terminal_pid_lengths[terminal_info.current_terminal]-1];
    }    
    processes_status[old_process] = 0;
    set_user_table(current_process);

    flush_tlb();

	uint32_t stack = KERNEL_STACK_START;
	if(current_process>0)
	{
    		stack = stack - PROC_KERNEL_LEN * current_process;
	}
    tss.esp0 = stack - 4;

    tss.ss0 = KERNEL_DS;
    asm volatile("movl %1,%%esp  \n\t"
		"movl %2,%%ebp  \n\t"
		"movl %3,%%eax  \n\t"
		"jmp ireturn_return  \n\t"
		: "=r" (i)
                : "r" (processes[old_process]->parent_esp), "r" (processes[old_process]->parent_ebp), "r" (ret_status));
    
	//sti();
return -1;
}
void get_command(uint8_t* filename, const uint8_t* command)
{
	int pos = 0;
 	if(filename == 0 || command == 0)
	{
		return;
	}
	for(pos = 0;pos < ARGS_BUFFER_SIZE-1;pos++)
	{
		if(command[pos] == '\0' || command[pos] == ' ' || command[pos] == '	' || command[pos] == '\n' || command[pos] == '\t')
		{
			filename[pos] = '\0';
			return;
		}else{
			filename[pos] = command[pos];
		}
	}
	filename[pos] = '\0';
	return;
}
uint32_t check_file(const uint8_t* command,uint32_t* starting_address,uint32_t* inode)
{
	uint8_t filename[ARGS_BUFFER_SIZE];
	get_command(filename, command);
	if(starting_address == 0 || inode == 0)
	{
		return -1;
	}
	dentry_t dentry;
	int read;
        uint8_t buf[BUFFER_SIZE_P];
        uint8_t buf2[4];
	if(read_dentry_by_name(filename, &dentry) == -1)
		return 0;

	read = read_data(dentry.inode_idx, 0, buf, BUFFER_SIZE_P,DONT_SKIP_NULLS);

	//check for executable magic number
	if(buf[0] != MAGIC_NUMBER_ONE || buf[1] != MAGIC_NUMBER_TWO || buf[2] != MAGIC_NUMBER_THREE || buf[3] != MAGIC_NUMBER_FOUR)
	{
		return 0;
	}
        //get starting address
	read_data( dentry.inode_idx, STARTING_POINT_LOCATION, buf2, STARTING_POINT_LENGTH,DONT_SKIP_NULLS);
        *starting_address = *((uint32_t*)buf2);
	*inode = dentry.inode_idx;
        return 1;
}

uint32_t load_file(uint32_t inode)
{
	int read;
        int not_done = 1;
        int count = 0;
        int position = 0;
        uint8_t buf[BUFFER_SIZE_P];

        while(not_done)
        {
		read = read_data(inode, position, buf, BUFFER_SIZE_P,DONT_SKIP_NULLS);
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

void flush_tlb()
{
	asm volatile("movl	%cr3,%eax  \n\t"
		"movl	%eax,%cr3  \n\t");
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

//for stdint/out, doesn't do anything
int32_t std_open(const uint8_t* filename, int32_t fd){
	return -1;
}

//for stdint/out, doesn't do anything
int32_t std_close(int32_t fd){
	return -1;
}

//for stdint/out, doesn't do anything
int32_t std_read(int32_t fd, uint8_t* buf, int32_t nbytes){
	return -1;
}

//for stdint/out, doesn't do anything
int32_t std_write(int32_t fd, const uint8_t* buf, int32_t nbytes){
	return -1;
}

//halts a program from an exception
void exception_halt()
{
	exception_halt_triggered = 1;
	syscall_halt(0);
}

void store_args(const uint8_t* command)
{
    if(command == 0)
    {
        return;
    }
    int count = 0;
    int buf_pos = 0;
    int started = 0;
    for(count = 0;count<ARGS_BUFFER_SIZE;count++)
    {
        if(started == 0 && command[count] == ' ')
        {
	     started = 1;
        }else if(started == 1 && command[count] != ' ' && command[count] != '	')
	{
	    started = 2;
	}
        if(command[count] == '\0' || command[count] == '\n')
	{
		processes[current_process]->args_len = buf_pos;
		return;
	}else if(started == 2){
 		processes[current_process]->args[buf_pos] = command[count];
		buf_pos++;
	}
    }
		processes[current_process]->args_len = buf_pos;
		return;
}


//code for switching between processes
void switch_process(int32_t term){
	if(term >= TERMINAL_NUMBER || term < 0)
	{
		return;
	}	
    if(terminal_info.terminal_pid_lengths[term] <= 0)
    {
        return;
    } 
    
    int process = terminal_info.terminal_pids[term][terminal_info.terminal_pid_lengths[term]-1];
    current_process = process;
    set_user_table(process);

    flush_tlb();

    set_process(term);

    //need to set stdin and stdout
    uint32_t stack = KERNEL_STACK_START;
    stack = stack - PROC_KERNEL_LEN * process;
    tss.esp0 = terminal_info.esp_locations[term];
    tss.ss0 = KERNEL_DS;
	
    int out;
    asm volatile("movl %1,%%esp \n\t"
			"jmp *%2 \n\t"
			"int $0x80 \n\t"
			:"=r" (out)
			:"r" (terminal_info.esp_locations[term]), "r" (terminal_info.jump_locations[term])
			);
    return;
}

void launch_base_shell() {
	 while(1)
    {
    int out;
        uint8_t file[10] = "shell";
	asm volatile("movl %1,%%ebx \n\t"
			"movl $2,%%eax \n\t"
			"int $0x80 \n\t"
			:"=r" (out)
			:"r" (&file)
			);
    }
}
