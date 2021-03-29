/* rtc_driver.h - Defines the rtc system driver
 * vim:ts=4 noexpandtab
 */


#ifndef _RTC_DRIVER_H
#define _RTC_DRIVER_H

#define MAX_RTC_DRIVERS 16
#define MAX_RTC_FREQUENCY 1024
#define TEN_ZERO_TWENTY_FOUR_HZ_RATE  6
#define TWO_HZ_RATE 15

#include "lib.h"
#include "i8259.h"
#include "rtc.h"
#ifndef ASM

typedef struct rtc_driver_instance {
    	int flag;
	int wait_ticks;
	int in_use;
} rtc_driver_instance_t;

extern rtc_driver_instance_t rtc_drivers_instances[MAX_RTC_DRIVERS];

//Opens an rtc driver and finds the first open instance for the calling process to use
int rtc_open(int* rtc_index);

//Waits for the next interrupt for this instance, then returns
int rtc_read(int rtc_index);

//Sets the interrupt frequency for this instance
int rtc_write(int rtc_index, int rate);

// Closes the given rtc index
int rtc_close(int rtc_index);


#endif /* ASM */
#endif /* _RTC_DRIVER_H */
