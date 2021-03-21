#ifndef _INTERRUPT_LINKAGE_H
#define _INTERRUPT_LINKAGE_H

#include "lib.h"
#include "interrupts.h"

//assembly linkage function for keyboard interrupt
extern void keyboard_wrapper();

//assembly linkage for RTC interrupt
extern void rtc_wrapper();


#endif //interrupt_linkage