
#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "lib.h"
#include "processes.h"

#define BUFFER_SIZE 128 //the maximum size of the buffer, in characters

#define NULLCHAR '\0' //representation of the nullcharacter

#define ENABLE 1 //enable the flag if terminal_read is called

#define DISABLE 0 //disable the flag if terminal_read is not invoked or finished

#define BEGINNING_IDX 0 //the beginning of the keyboard_buffer (index)

#define MEM_BUF_SIZE 4096 //size of the video memory

#define TERMINAL_NUMBER 3 //Number of terminals running

#define NUM_CURSORS 2  //Number of cursors

#define MAX_TERMINAL_PROCESSES 4
#ifndef ASM

uint8_t buffer[BUFFER_SIZE];

uint32_t terminal_read_flag; //flag to track if the terminal_read function has been invoked or not

// uint8_t buf[BUFFER_SIZE];

typedef struct terminal_info {
    uint32_t current_terminal;
    uint32_t display_terminal;
    uint32_t active_terminals[TERMINAL_NUMBER];
    uint8_t vid_mem_buffer[TERMINAL_NUMBER][MEM_BUF_SIZE];
    uint8_t cursors[TERMINAL_NUMBER][NUM_CURSORS];
    uint8_t keyboard_buffers[TERMINAL_NUMBER][BUFFER_SIZE];
    uint32_t keyboard_buffer_indexes[TERMINAL_NUMBER];
    uint32_t terminal_pids[TERMINAL_NUMBER][MAX_TERMINAL_PROCESSES];
    uint32_t terminal_pid_lengths[TERMINAL_NUMBER];
    uint32_t esp_locations[TERMINAL_NUMBER];
    uint32_t jump_locations[TERMINAL_NUMBER];
} terminal_info_t;

//terminal tracker structure for multiple terminals
terminal_info_t terminal_info;

//open the terminal, initialize
extern int32_t terminal_open(const uint8_t* filename);

//close terminal
extern int32_t terminal_close(int32_t fd);

//read from keyboard buffer, into terminal buffer, return number of bytes read
extern int32_t terminal_read(int32_t fd, uint8_t* buf, int32_t nbytes);

//output from terminal buf onto screen
extern int32_t terminal_write(int32_t fd, const uint8_t* buf, int32_t nbytes);

//swap the current terminal to new_terminal
extern void terminal_swap(int32_t new_terminal, uint32_t esp_location, uint32_t jump_location);

//initialize terminal structure variables
extern void init_terminal();

//save the current terminal data when switching terminals
extern void save_terminal(uint32_t esp_location, uint32_t jump_location);

//load in the new terminal mem and/or launch new shell
extern void load_terminal(uint32_t term);

#endif /* ASM */

#endif //TERMINAL_H
