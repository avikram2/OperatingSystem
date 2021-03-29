#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "lib.h"
#include "i8259.h"
#include "rtc_driver.h"

#define SCAN_CODE_SIZE 0x35 //size of current array storing characters indexed by scancode
#define DATA_PORT_KEYBOARD 0x60 //port to read data from PS2 device
#define KEYBOARD_IRQ_1 1 //irq number for keyboard

#define RTC_IRQ_NUM 8 //irq number for rtc 

#define ORIGIN_CURSOR 0 //mask to update cursor back to the beginning of the screen

#define BREAK_MASK  0x80 //Bit mask for chekcing for break scancodes
#define SCAN_MASK   0x7F //Bit mask for looking at just the index of the code

#define BACK_SPACE  0x0E //backspace scancode
#define ENTER       0x1C //Enter scancode
#define LEFT_CRTL   0x1D //LCTRL scancode
#define LEFT_SHIFT  0x2A //Left Shift scancode
#define RIGHT_SHIFT 0x36 //Right Shift scancode
#define LEFT_ALT    0x38 //LALT scancode
#define CAPS_LOCK   0x3A //Caps scancode

#define NUM_COLS    80 //macros for RTC handler
#define NUM_ROWS    25
#define VIDEO       0xB8000
#define REGISTER_C  0x0C
#define RTC_PORT    0x70 
#define CMOS_PORT   0x71 //register A,B,C port in RTC

#ifndef ASM

//interrupt handler for the keyboard
extern void interrupt_keyboard_handler();

//rtc interrupt handler
extern void rtc_handler();

#endif

#endif
