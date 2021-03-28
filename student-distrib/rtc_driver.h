/* rtc_driver.h - Defines the rtc system driver
 * vim:ts=4 noexpandtab
 */


#ifndef _RTC_DRIVER_H
#define _RTC_DRIVER_H

#include "lib.h"
#include "i8259.h"

static uint32_t interrupted_flag;

void rtc_open();

void rtc_read();

void rtc_write(uint32_t rate);

void rtc_close();

void set_rtc_rate(uint32_t rate);

#endif /* _RTC_DRIVER_H */
