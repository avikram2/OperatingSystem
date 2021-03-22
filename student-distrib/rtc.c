
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
    enable_irq(RTC_LINE_IRQ); //enable RTC interrupts
    return;
}
