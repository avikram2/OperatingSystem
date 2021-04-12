
#include "syscalls.h"


void * rtc_ops[4] = {rtc_open, rtc_close, rtc_read, rtc_write};

void * dir_ops[4] = {directory_open, directory_close, directory_read, directory_write};

void * file_ops[4] = {file_open, file_close, file_read, file_close};



// read:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes){
    return -1;
}

// write:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
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
    if (pid < 0 || pid > 1) //if the current process id is out of bounds return -1
    return -1;

    int fda_index = 0; //variable to hold current index in the fda while finding open spot

    for (fda_index = 0; fda_index < NUMBER_OF_FILE_DESCRIPTORS; ++fda_index){
        //traverse through FD array
        if (processes[pid]->file_descriptors[fda_index].flags != ACTIVE_FLAG){
            processes[pid]->file_descriptors[fda_index].flags = ACTIVE_FLAG;
            processes[pid]->file_descriptors[fda_index].inode = dentry.inode_idx;

            switch(dentry.type){
                case 0:
                processes[pid]->file_descriptors[fda_index].operations_table = rtc_ops;
                break;
                case 1:
                processes[pid]->file_descriptors[fda_index].operations_table = dir_ops;
                break;
                case 2:
                processes[pid]->file_descriptors[fda_index].operations_table = file_ops;
                break;
                default:
                return -1;
            }

            int ret = processes[pid]->file_descriptors[fda_index].operations_table[0](filename);
            if (ret == -1)
            return -1;

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
    return -1;
}

// getargs:
// tbd
//inputs: tbd
//output: tbd
//effect: tbd
int32_t syscall_getargs(uint8_t* buf, int32_t nbytes){
    return -1;
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



