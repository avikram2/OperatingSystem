
#include "terminal.h"


//terminal_open
//function to initialize buffers and the terminal
//inputs: filename: unsigned int pointer
//outputs: return 0
//effects initializes keyboard_buffer
int32_t terminal_open(const uint8_t* filename){
    int i;
    keyboard_buffer_index = 0; //reset to beginning of keyboard buffer
    for (i = 0; i < BUFFER_SIZE; ++i){ //initalize keyboard buffer
        keyboard_buffer[i] = '\0'; //initialize to null character
        buffer[i] = '\0';
    }

    return 0;
}


//terminal_read
//read the provided number of bytes from the keyboard buffer into the provided terminal buffer
//inputs: buf-- the provided terminal buffer to read the keyboard buffer into, nbytes -- the amount of bytes to read
int32_t terminal_read(int32_t fd, uint8_t* buf, int32_t nbytes){
    terminal_read_flag = 1; //enable flag, which means read function invoked
    while (1){
    //polling for new_line to arrive in buffer
    if (keyboard_buffer_index != 0 && keyboard_buffer[keyboard_buffer_index-1] == '\n'){
    int i = 0;
    for (i = 0; i < nbytes; i++){ //for each byte
        buf[i] = keyboard_buffer[i]; //set terminal buffer equal to keyboard buffer
    }
    keyboard_buffer_index = 0; //reset the buffer
    terminal_read_flag = 0; //disable the flag, exiting the read function

    for (i = 0; i < BUFFER_SIZE; ++i){ //initalize keyboard buffer
        keyboard_buffer[i] = '\0'; //initialize to space
    }
    return nbytes;
    }

    }

    terminal_read_flag = 0; //disable the flag, exiting the read function
    return 0;
}



//terminal_write
//write the provided number of bytes to the screen from the terminal buffer, provided as argument
//inputs: buf -- the buffer from which to write to the screen, nbytes: how many bytes to write to the screen
//output: how many characters written to screen
int32_t terminal_write(int32_t fd, uint8_t* buf, int32_t nbytes){
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
    return 0; //return 0, close terminal
}

