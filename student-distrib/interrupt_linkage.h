#ifndef _INTERRUPT_LINKAGE_H
#define _INTERRUPT_LINKAGE_H

#include "interrupts.h"


#ifndef ASM


//assembly linkage function for keyboard interrupt
extern void keyboard_wrapper();

extern void rtc_wrapper();

#endif //ASM

#endif //interrupt_linkage
