
#include "terminal.h"
#include "types.h"


//terminal_open
//function to initialize buffers and the terminal, open the terminal
//inputs: filename: unsigned integer pointer
//outputs: return 0
//effects initializes keyboard_buffer, and opens the terminal
int32_t terminal_open(const uint8_t* filename){
    int i;
    terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal] = BEGINNING_IDX; //reset to beginning of keyboard buffer
    for (i = 0; i < BUFFER_SIZE; ++i){ //initalize keyboard buffer
        terminal_info.keyboard_buffers[terminal_info.current_terminal][i] = NULLCHAR; //initialize to null character
        buffer[i] = NULLCHAR;
    }

    return 0;
}


//terminal_read
//read the provided number of bytes from the keyboard buffer into the provided terminal buffer
//inputs: buf-- the provided terminal buffer to read the keyboard buffer into, nbytes -- the amount of bytes to read
int32_t terminal_read(int32_t fd, uint8_t* buf, int32_t nbytes){
    terminal_read_flag = ENABLE; //enable flag, which means read function invoked

    if (nbytes <= BEGINNING_IDX){ //if incorrect input (negative bytes)
        terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal] = BEGINNING_IDX; //reset the keyboard buffer
        terminal_read_flag = DISABLE; //exiting function

        int j;
        for (j = 0; j < BUFFER_SIZE; ++j){ //reset keyboard buffer
        terminal_info.keyboard_buffers[terminal_info.current_terminal][j] = NULLCHAR; //reset to nullcharacter
        }


        return 0;
    }

    while (1){
    //polling for new_line character to arrive in the buffer
    if (terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal] != 0 && terminal_info.keyboard_buffers[terminal_info.current_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal]-1] == '\n'){
    int returnvalue = (nbytes < terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal])? nbytes: terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal];
    //the returnvalue (aka number of bytes read by the function)
    int i = 0;
    for (i = 0; i < returnvalue; i++){ //for each byte
        buf[i] = terminal_info.keyboard_buffers[terminal_info.current_terminal][i]; //set terminal buffer equal to keyboard buffer
    }

    for (i = 0; i < BUFFER_SIZE; ++i){ //reset keyboard buffer
        terminal_info.keyboard_buffers[terminal_info.current_terminal][i] = NULLCHAR; //reset to nullcharacter
    }
    //calculate the number of bytes printed

    terminal_info.keyboard_buffer_indexes[terminal_info.current_terminal] = BEGINNING_IDX; //reset the buffer index
    terminal_read_flag = DISABLE; //disable the flag, exiting the read function
    uint8_t c = '\n';
    putc(c);
    return returnvalue;
    }

    }

    terminal_read_flag = DISABLE; //disable the flag, exiting the read function
    return 0;
}



//terminal_write
//write the provided number of bytes to the screen from the terminal buffer, provided as argument
//inputs: buf -- the buffer from which to write to the screen, nbytes: how many bytes to write to the screen
//output: how many characters written to screen
int32_t terminal_write(int32_t fd, const uint8_t* buf, int32_t nbytes){
//cat doesn't work if we compare a max number of bytes
    if (nbytes <= BEGINNING_IDX){//|| nbytes > BUFFER_SIZE){ //if the number of bytes is negative or too big, return
    return 0;
    }
    int i = 0;
    for (i = 0; i < nbytes; i++){
        putc(buf[i]); //print character to screen
    }
    update_cursor(get_cursor_x(), get_cursor_y()); //update cursor after printing

    return nbytes;
}




//terminal_close
//function to close the terminal driver
int32_t terminal_close(int32_t fd){
    return -1; //return 0, close terminal
}



//Program for initializing the terminal info structure
void init_terminal(){
  //set current terminal to 0 upon boot
  terminal_info.current_terminal = 0;
  //only first terminal active on boot
  terminal_info.active_terminals[0] = 1;
  terminal_info.active_terminals[1] = 0;
  terminal_info.active_terminals[2] = 0;
  set_buffers(terminal_info.vid_mem_buffer[0], terminal_info.vid_mem_buffer[1],terminal_info.vid_mem_buffer[2]);
  return;
}


//terminal swqapping function
//takes interger for terminal (0-2) and saves current while swapping to the next
void terminal_swap(int32_t new_terminal){
    if(new_terminal == terminal_info.current_terminal){
      //printf("Already on this terminal\n");
      return;
    }
    //save the current contents of the terminal
    save_terminal();
    //load the new terminal
    load_terminal(new_terminal);
    return;
}


//save the terminal data before switching to a new terminal
void save_terminal(){
  // copy curent vid_mem to terminal structure
  memcpy(terminal_info.vid_mem_buffer[terminal_info.current_terminal], (char *)VIDEO, MEM_BUF_SIZE);
  //save current curors
  terminal_info.cursors[terminal_info.current_terminal][0] = get_cursor_x();
  terminal_info.cursors[terminal_info.current_terminal][1] = get_cursor_y();
}


//load in the new terminal vid_mem and shell
void load_terminal(uint32_t term){
  //change current cursors to the new terminal
  update_cursor(terminal_info.cursors[term][0], terminal_info.cursors[term][1]);
  //swap the vid memory to the new terminal
  memcpy((char *)VIDEO, terminal_info.vid_mem_buffer[term], MEM_BUF_SIZE);
  //change current terminal to new terminal
  terminal_info.current_terminal = term;
  set_display(term);
  if(terminal_info.active_terminals[term]){
    switch_process(0); //switch to a new process
    //printf("The terminal shell previously launched\n");
  }else{
    //set active terminal flag
    terminal_info.active_terminals[term] = 1;
    //launch a new shell for the terminal
    syscall_execute("shell");
  }
}
