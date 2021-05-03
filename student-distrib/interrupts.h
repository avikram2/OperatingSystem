#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "lib.h"
#include "i8259.h"
#include "rtc_driver.h"
#include "terminal.h"
#include "processes.h"

#define SCAN_CODE_SIZE 0x35 //size of current array storing characters indexed by scancode
#define DATA_PORT_KEYBOARD 0x60 //port to read data from PS2 device
#define KEYBOARD_IRQ_1 1 //irq number for keyboard

#define RTC_IRQ_NUM 8 //irq number for rtc

#define ORIGIN_CURSOR 0 //mask to update cursor back to the beginning of the screen

#define BREAK_MASK  0x80 //Bit mask for checking for break scancodes
#define SCAN_MASK   0x7F //Bit mask for looking at just the index of the code

#define BACK_SPACE  0x0E //backspace scancode
#define ENTER       0x1C //Enter scancode
#define LEFT_CRTL   0x1D //LCTRL scancode
#define LEFT_SHIFT  0x2A //Left Shift scancode
#define RIGHT_SHIFT 0x36 //Right Shift scancode
#define LEFT_ALT    0x38 //LALT scancode
#define CAPS_LOCK   0x3A //Caps scancode pressed
#define SPACE       0x39 //Space scancode
#define ESCAPE      0x01 //escape key scancode
#define ZERO_CODE   0x00 //null (zero) macro
#define F1          0x3B //Function 1-6 keycodes
#define F2          0x3C
#define F3          0x3D
#define F4          0x3E
#define F5          0x3F
#define F6          0x40

#define SCHEDULING_COUNT  25 //in Hertz
#define PIT_RELOAD_COUNT 1193186/SCHEDULING_COUNT //The oscillator used by the PIT chip runs at (roughly) 1.193182 MHz

#define NUM_COLS    80 //macros for RTC handler
#define NUM_ROWS    25
#define VIDEO       0xB8000
#define REGISTER_C  0x0C
#define RTC_PORT    0x70
#define CMOS_PORT   0x71 //register A,B,C port in RTC


#define PIT_IRQ_NUM 0

/*
I/O port     Usage
0x40         Channel 0 data port (read/write)
0x41         Channel 1 data port (read/write)
0x42         Channel 2 data port (read/write)
0x43         Mode/Command register (write only, a read is ignored)
*/
#define PIT_CH0DATA 0x40  //pit channel 0 data port
#define PIT_CMD  0x43 //pit command register port

#define PIT_SQUAREWAVE_MODE 0x36 //CHANNEL0, SQUAREWAVE GENERATOR MODE (MODE 3)

#define PIT_UPPER_BITMASK 0xFF

#define PIT_BITMASK 0xFF00

#define PIT_BITSHIFT 8

#ifndef ASM


//interrupt handler for the keyboard
extern void interrupt_keyboard_handler(uint32_t jump_location,uint32_t esp_location);

//rtc interrupt handler
extern void rtc_handler(uint32_t jump_location,uint32_t esp_location);

//pit interrupt handler
extern void handle_possible_scheduling(uint32_t jump_location,uint32_t esp_location);

//pit initialization
extern void init_pit();
#endif

#endif
