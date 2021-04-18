
#include "syscalls.h"


static fops_t rtc_ops = {rtc_open, rtc_close, rtc_read, rtc_write};
static fops_t dir_ops = {directory_open, directory_close, directory_read, directory_write};
static fops_t file_ops = {file_open, file_close, file_read, file_write};



// read:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes){
    int pid = get_pid();
    if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();
    if(processes[pid]->file_descriptors[fd].flags != ACTIVE_FLAG)
    {
        return -1;
    }
    return processes[pid]->file_descriptors[fd].fops_table->read(fd,buf,nbytes);
}

// write:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes){
    int pid = get_pid();
    if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();
    if(processes[pid]->file_descriptors[fd].flags != ACTIVE_FLAG)
    {
        return -1;
    }
    return processes[pid]->file_descriptors[fd].fops_table->write(fd,buf,nbytes);
}

// open:
// allocates an entry in the FDA of the current process for the file
//inputs: filename - name of the file
//output: fda array index
//effect: allocates entry in the FDA for the filename, fills in FOPS
int32_t syscall_open(const uint8_t* filename){
    dentry_t dentry; //directory entry object

    if(read_dentry_by_name(filename, &dentry) == -1)
		return -1; //file not found

    int pid = get_pid();
    if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;

    int fda_index = 0; //variable to hold current index in the fda while finding open spot

    pcb_t** processes = get_process();

    for (fda_index = 0; fda_index < NUMBER_OF_FILE_DESCRIPTORS; ++fda_index){
        //traverse through FD array
        if (processes[pid]->file_descriptors[fda_index].flags != ACTIVE_FLAG){
            processes[pid]->file_descriptors[fda_index].flags = ACTIVE_FLAG;
            processes[pid]->file_descriptors[fda_index].inode = dentry.inode_idx;

            switch(dentry.type){
                case 0:
		processes[pid]->file_descriptors[fda_index].fops_table = &rtc_ops;
                //processes[pid]->file_descriptors[fda_index].operations_table->read = rtc_read;
                //processes[pid]->file_descriptors[fda_index].operations_table->write = rtc_write;
                //processes[pid]->file_descriptors[fda_index].operations_table->open = rtc_open;
                //processes[pid]->file_descriptors[fda_index].operations_table->close = rtc_close;
                break;
                case 1:
		processes[pid]->file_descriptors[fda_index].fops_table = &dir_ops;
                //processes[pid]->file_descriptors[fda_index].operations_table->read = directory_read;
                //processes[pid]->file_descriptors[fda_index].operations_table->write = directory_write;
                //processes[pid]->file_descriptors[fda_index].operations_table->open = directory_open;
                //processes[pid]->file_descriptors[fda_index].operations_table->close = directory_close;
                break;
                case 2:
		processes[pid]->file_descriptors[fda_index].fops_table = &file_ops;
                //processes[pid]->file_descriptors[fda_index].operations_table->read = file_read;
                //processes[pid]->file_descriptors[fda_index].operations_table->write = file_write;
                //processes[pid]->file_descriptors[fda_index].operations_table->open = file_open;
                //processes[pid]->file_descriptors[fda_index].operations_table->close = file_close;
                break;
                default:
                return -1;
            }

            int ret = processes[pid]->file_descriptors[fda_index].fops_table->open(filename,fda_index);
            //int ret = processes[pid]->file_descriptors[fda_index].operations_table->open(filename);
            if (ret == -1)
	    {
                processes[pid]->file_descriptors[fda_index].flags = INACTIVE_FLAG;
            	return -1;
            }

            return fda_index;
        }

    }
return -1; 


}

// close:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_close(const uint32_t fd){
    int pid = get_pid();
    if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();
    if(processes[pid]->file_descriptors[fd].flags != ACTIVE_FLAG)
    {
        return -1;
    }
    return processes[pid]->file_descriptors[fd].fops_table->close(fd);
}

// getargs:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_getargs(uint8_t* buf, int32_t nbytes){
    if(buf == 0 || nbytes<= 1)
	return -1;
    int pid = get_pid();
    if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    pcb_t** processes = get_process();
    int arg_len = processes[pid]->args_len;
    //plus 1 for null terminator
    if(arg_len + 1 > nbytes)
    	return -1;
    if(arg_len <= 0 && nbytes > 0)
    {
	buf[0] = '\0';
    	return 0;
    }
    int pos = 0;
    for(pos = 0; pos < arg_len; pos++)
    {
         buf[pos] = processes[pid]->args[pos];
    }
    buf[pos] = '\0';
    return 0;
}

// vidmap:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_vidmap(uint8_t** screen_start){
    return -1;
}

// set_handler:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_set_handler(int32_t signum, void* handle_address){
    return -1;
}
// sigreturn:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_sigreturn(void){
    return -1;
}



