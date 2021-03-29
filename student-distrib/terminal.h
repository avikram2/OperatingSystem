
#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "lib.h"

#define BUFFER_SIZE 128 //the maximum size of the buffer, in characters

#define NULLCHAR '\0' //representation of the nullcharacter

#define ENABLE 1 //enable the flag if terminal_read is called

#define DISABLE 0 //disable the flag if terminal_read is not invoked or finished

#ifndef ASM

uint8_t keyboard_buffer[BUFFER_SIZE];

uint8_t buffer[BUFFER_SIZE];

uint32_t keyboard_buffer_index; //tracks current index of keyboard buffer

uint32_t terminal_read_flag;

// uint8_t buf[BUFFER_SIZE];

//open the terminal, initialize
extern int32_t terminal_open(const uint8_t* filename);

//close terminal
extern int32_t terminal_close(int32_t fd);

//read from keyboard buffer, into terminal buffer, return number of bytes read
extern int32_t terminal_read(int32_t fd, uint8_t* buf, int32_t nbytes);

//output from terminal buf onto screen
extern int32_t terminal_write(int32_t fd, uint8_t* buf, int32_t nbytes);

#endif /* ASM */

#endif //TERMINAL_H
