
#include "rtc.h"
#include "i8259.h"
/*  Description: Initialize the RTC and enable interrupts on IRQ8.
 *  Input: None
 *  Output: None
 *  Effect: initialize RTC and enable RTC interrupts
 */
void init_rtc(){
    //From OSDev
    outb(REG_B, REG_SELECT);      //Select register B (0xB), and disable NMI (0x80)
    char prev = inb(DATA_PORT);   //Read the current value of register B
    outb(REG_B, REG_SELECT);      //Set the index again (a read will reset the index to register D)
    outb(prev | 0x40, DATA_PORT); //update value from data port and send again
    
    set_rtc_rate(TWO_HZ_RATE);      //Set the RTC interrupt rate to 1024Hz by default
    
    enable_irq(RTC_LINE_IRQ); //enable RTC interrupts

    return;
}

/*  Description: Sets the RTC interrupt frequency
 *  Input: An integer between 3 and 15 that represents the interrupt rate
 *  Output: None
 *  Effect: initialize RTC and enable RTC interrupts
 */
void set_rtc_rate(int rate)
{
	//From OSDev
	rate &= LOWER_4_BITS_MASK;			//The rate must be less than 15
	outb(REG_A,REG_SELECT);		// set index to register A, disable NMI
	char prev_a_value = inb(DATA_PORT);	// get initial value of register A
	outb(REG_B, REG_SELECT);		// reset index to A
	outb((prev_a_value & UPPER_4_BITS_MASK) | rate,DATA_PORT); //write only our rate to A. Note, rate is the bottom 4 bits.
}
