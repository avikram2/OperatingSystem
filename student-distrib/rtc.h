#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "lib.h"

#define REG_SELECT      0x70 //ports for the RTC and CMOS
#define DATA_PORT       0x71
#define REG_A           0x8A //select register, disable NMI 
#define REG_B           0x8B

#define TWO_HZ_RATE 15
#define TEN_ZERO_TWENTY_FOUR_HZ_RATE  6

#define LOWER_4_BITS_MASK   0x0F
#define UPPER_4_BITS_MASK   0xF0

#define RTC_LINE_IRQ    8    //irq line number for rtc

#ifndef ASM

//initialize the rtc and enable rtc interrupts
extern void init_rtc();

//Sets the RTC interrupt frequency
extern void set_rtc_rate(int rate);

#endif /* ASM */
#endif /* _RTC_H  */
