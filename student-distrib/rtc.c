
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
