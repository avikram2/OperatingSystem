/* exceptions.c - Implements the various idt exception handlers
 * vim:ts=4 noexpandtab
 */

#include "rtc_driver.h"

//static uint32_t interrupted_flag = 0;

void rtc_open()
{
	interrupted_flag = 0;
	set_rtc_rate(2);
	//return 0;
}

void rtc_read()
{
	interrupted_flag = 1;
	while(interrupted_flag == 1)
	{

	}
	//return 0;

}

void rtc_write(uint32_t rate)
{
	/*if((rate & (rate-1)) != 0 || rate > 1024)
	{
		return -1;
	}*/
	set_rtc_rate(rate);
	//return 0;
}

void rtc_close()
{
	interrupted_flag = 0;
	set_rtc_rate(2);
	//return 0;
}

void set_rtc_rate(uint32_t rate2)
{
	cli();
	uint32_t rate = 15;
	rate &= 0x0F;			// rate must be above 2 and not over 15
outb(0x8A,0x70);		// set index to register A, disable NMI
char prev=inb(0x71);	// get initial value of register A
outb(0x8A, 0x70);		// reset index to A
outb((prev & 0xF0) | rate,0x71); //write only our rate to A. Note, rate is the bottom 4 bits.
	sti();
}

