#ifndef _RTC_H
#define _RTC_H

#include "types.h"


#define REG_SELECT      0x70
#define DATA_PORT       0x71
#define REG_A           0x8A
#define REG_B           0x8B


extern void init_rtc();

#endif
