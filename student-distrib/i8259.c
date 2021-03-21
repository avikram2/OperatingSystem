/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {


    //first control word
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    //second control word
    outb(ICW2_MASTER, PIC1_DATA);
    outb(ICW2_SLAVE, PIC2_DATA);


    //third control word
    outb(ICW3_MASTER, PIC1_DATA);
    outb(ICW3_SLAVE, PIC2_DATA);

    //fourth control word

    outb(ICW4, PIC1_DATA);
    outb(ICW4, PIC2_DATA);


    //mask all interrupts

    outb(irq_mask, PIC1_DATA);
    outb(irq_mask, PIC2_DATA);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {

    if (((irq_num & 8) == 1) && (irq_num < 16)){ //belongs to the slave

        uint32_t mask = ~(1 << (irq_num-8));
        uint8_t new_mask = (irq_mask&mask);
        uint32_t enable_slave_mask = ~(1 << (2));
        uint8_t enable_slave = (enable_slave_mask & irq_mask);
        outb(enable_slave, PIC1_DATA);
        outb(new_mask, PIC2_DATA);
    }

    else if (irq_num < 8){ //belongs to master
        uint32_t mask = ~(1 << (irq_num));
        uint8_t new_mask = (irq_mask&mask);
        outb(new_mask, PIC1_DATA);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
}
