#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "lib.h"

#define REG_SELECT      0x70 //ports for the RTC and CMOS
#define DATA_PORT       0x71
#define REG_A           0x8A //select register, disable NMI 
#define REG_B           0x8B


#define RTC_LINE_IRQ    8    //irq line number for rtc


//initialize the rtc and enable rtc interrupts
extern void init_rtc();

#endif
