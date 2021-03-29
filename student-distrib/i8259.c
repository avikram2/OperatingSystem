/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {


    //first control word, except three more
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    //second control word, correct port offsets
    outb(ICW2_MASTER, PIC1_DATA);
    outb(ICW2_SLAVE, PIC2_DATA);


    //third control word, where cascading occurs
    outb(ICW3_MASTER, PIC1_DATA);
    outb(ICW3_SLAVE, PIC2_DATA);

    //fourth control word, 0x01

    outb(ICW4, PIC1_DATA);
    outb(ICW4, PIC2_DATA);


    //mask all interrupts

    master_mask = 0xFF;
    slave_mask = 0xFF; //mask all lines
    outb(master_mask, PIC1_DATA);
    outb(slave_mask, PIC2_DATA);
}

/* enable_irq
Enable (unmask) the specified IRQ line
inputs: irq_num - the IRQ line number
output: none
side effect: irq line unmasked
 */
void enable_irq(uint32_t irq_num) {

    if ((irq_num >= 8) && (irq_num < 16)){ //belongs to the slave

        uint32_t mask = ~(1 << (irq_num-8)); //unmask correct line on slave, 8 ports on MASTER
        slave_mask = (slave_mask&mask);
        uint32_t enable_slave_mask = ~(1 << (2)); //unable IRQ 2 on master as well
        master_mask = (enable_slave_mask & master_mask);
        outb(master_mask, PIC1_DATA);
        outb(slave_mask, PIC2_DATA);
    }

    else if (irq_num < 8){ //belongs to master
        uint32_t mask = ~(1 << (irq_num)); //unmask correct line on master 
        master_mask = (master_mask&mask);
        outb(master_mask, PIC1_DATA);
    }
}
// disable_irq
/* Disable (mask) the specified IRQ
inputs: irq_num: the irq line
output: none
side effect: disables that irq line
 */
void disable_irq(uint32_t irq_num) {

    if ((irq_num >= 8) && (irq_num < 16)){ //belongs to the slave
        uint32_t mask = (1 << (irq_num-8)); //mask proper line, subtracting by 8
        slave_mask = (slave_mask|mask);
        outb(slave_mask, PIC2_DATA);
    }

    else if (irq_num < 8){ //line is in master 
    uint32_t mask = (1 << (irq_num)); //mask proper line
    master_mask = (master_mask|mask);
    outb(master_mask, PIC1_DATA);
    }

}
//send_eoi
/* Send end-of-interrupt signal for the specified IRQ 
input: irq_num; irq line to send the eoi signal to
output: none
side effect: eoi signal sent to proper line, interrupts unmasked
*/
void send_eoi(uint32_t irq_num) {

    uint8_t eoisignal = EOI;
    if ((irq_num >= 8)&& (irq_num < 16)){ //belongs to the slave
    uint8_t eoi_slave = eoisignal + (uint8_t)(irq_num&7); //proper masked eoi signal depending on irq line
    outb(eoi_slave, SLAVE_8259_PORT); //send eoi to slave line
    outb(eoisignal + ICW3_SLAVE, MASTER_8259_PORT); //send eoi to IRQ on master 
    }

    else if (irq_num < 8){ //line belongs to master 
        uint8_t eoi_master = eoisignal + (uint8_t)(irq_num); //proper eoi signal depending on irq number
        outb(eoi_master, MASTER_8259_PORT); //send to master
    }
}
