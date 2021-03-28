#include "rtc.h"
#include "i8259.h"

/*  Description: Initialize the RTC to IRQ8.
 *  Input: None
 *  Output: None
 *  Effect: initialize RTC and set bitmask for PIC
 */
void init_rtc(){
    //From OSDev
    outb(REG_B, REG_SELECT);      //Select register B (0xB), and disable NMI (0x80)
    char prev = inb(DATA_PORT);   //Read the current value of register B
    outb(REG_B, REG_SELECT);      //Set the index again (a read will reset the index to register D)
    outb(prev | 0x40, DATA_PORT); //
    enable_irq(8); //enable RTC interrupts
    return;
}

/*  Description: Change the RTC frequency.
 *  Input: frequency to change (2-1024)
 *  Output: None
 *  Effect: Alter the rate of RTC interrupts.
 */
void set_freq(int freq){
  freq &= RATE_MASK;			// ensuring rate is between 2 and 10 (2-1024)
  outb(REG_A, REG_SELECT);		// set index to register A, disable NMI
  char prev = inb(DATA_PORT);	// get initial value of register A
  outb(REG_A, REG_SELECT);		// select A again
  outb((prev & 0xF0) | freq, DATA_PORT);  //Write the rate to the bottom 4 bits
  return;
}

int32_t rtc_read(){

}

int32_t rtc_write(){

}

int32_t rtc_open(){

  return 0;
}

int32_t rtc_close(){

}
