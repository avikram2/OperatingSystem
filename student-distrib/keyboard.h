#ifndef _KEYBOARD_H
#define _KEYBOARD_H


#include "i8259.h"

#define KEYBOARD_IRQ_LINE 1


//initialize the keyboard device, enable keyboard interrupts
extern void keyboard_init();



#endif //_KEYBOARD_H
