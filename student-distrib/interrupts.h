
#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "lib.h"
#include "i8259.h"


#define SCAN_CODE_SIZE 0x35 //size of current array storing characters indexed by scancode
#define DATA_PORT 0x60 //port to read data from PS2 device
#define KEYBOARD_IRQ_1 1 //irq number for keyboard

#define RTC_IRQ_NUM 8

#define NUM_COLS    80 //macros for RTC handler
#define NUM_ROWS    25
#define VIDEO       0xB8000
#define REGISTER_C  0x0C
#define RTC_PORT    0x70
#define CMOS_PORT   0x71

//interrupt handler for the keyboard
extern void interrupt_keyboard_handler();

extern void rtc_handler();



#endif