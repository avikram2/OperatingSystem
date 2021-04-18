/* rtc_driver.h - Defines the rtc system driver
 * vim:ts=4 noexpandtab
 */


#ifndef _RTC_DRIVER_H
#define _RTC_DRIVER_H

#define MAX_RTC_DRIVERS 16
#define MAX_RTC_FREQUENCY 512
#define TEN_ZERO_TWENTY_FOUR_HZ_RATE  6
#define TWO_HZ_RATE 15

#include "lib.h"
#include "i8259.h"
#include "rtc.h"
#include "processes.h"
#ifndef ASM

typedef struct rtc_driver_instance {
    	int flag;
	int wait_ticks;
	int in_use;
	int set;
} rtc_driver_instance_t;

extern rtc_driver_instance_t rtc_drivers_instances[MAX_RTC_DRIVERS];

//Opens an rtc driver and finds the first open instance for the calling process to use
int rtc_open(const uint8_t* fname, uint32_t fd);

//Waits for the next interrupt for this instance, then returns
int rtc_read(uint32_t fd, uint8_t* buf, int32_t nbytes);

//Sets the interrupt frequency for this instance
int rtc_write(uint32_t fd, const uint8_t* buf, int32_t nbytese);

// Closes the given rtc index
int rtc_close(uint32_t fd);


#endif /* ASM */
#endif /* _RTC_DRIVER_H */
