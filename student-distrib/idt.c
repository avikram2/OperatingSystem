/* idt.c - Implements the functions to load the idt table
 * vim:ts=4 noexpandtab
 */

#include "idt.h"

/* Sets the idt entry for each index in the idt table */
void load_idt()
{
    printf("loading idt\n");

    set_exception_irq(0,divide_exception);
    set_exception_irq(1,debug_exception);
    set_exception_irq(2,nmi_exception);
    set_exception_irq(3,breakpoint_exception);
    set_exception_irq(4,overflow_exception);
    set_exception_irq(5,bound_range_exception);
    set_exception_irq(6,invalid_opcode_exception);
    set_exception_irq(7,device_not_available_exception);
    set_exception_irq(8,double_fault_exception);
    set_exception_irq(9,coprocessor_segment_overrun_exception);
    set_exception_irq(10,invalid_tss_exception);
    set_exception_irq(11,segment_not_present_exception);
    set_exception_irq(12,stack_fault_exception);
    set_exception_irq(13,general_protection_exception);
    set_exception_irq(14,page_fault_exception);
    set_exception_irq(16,floating_point_exception);
    set_exception_irq(17,alignment_check_exception);
    set_exception_irq(18,machine_check_exception);
    set_exception_irq(19,simd_floating_point_exception);
    set_exception_irq(20,virtualization_exception);
    set_exception_irq(21,control_protection_exception);

    //set keyboard and rtc interrupts
    set_interrupt_irq(0x21, keyboard_wrapper);
    set_interrupt_irq(0x28, rtc_wrapper);
}

/* Sets up the correct values for the given idt index(irq) and sets it to point to the given handler function */
void set_exception_irq(int irq, void * handler)
{
    idt[irq].size = 1;
    idt[irq].dpl = 0;
    idt[irq].seg_selector = 16;
    idt[irq].present = 1;
    idt[irq].reserved4 = 0;
    idt[irq].reserved3 = 1;
    idt[irq].reserved3 = 1;
    idt[irq].reserved2 = 1;
    idt[irq].reserved1 = 1;
    SET_IDT_ENTRY(idt[irq],handler);
}


void set_interrupt_irq(int irq, void * handler){
    idt[irq].size = 1;
    idt[irq].dpl = 0;
    idt[irq].seg_selector = 16;
    idt[irq].present = 1;
    idt[irq].reserved4 = 0;
    idt[irq].reserved3 = 1;
    idt[irq].reserved3 = 1;
    idt[irq].reserved2 = 1;
    idt[irq].reserved1 = 1;
    SET_IDT_ENTRY(idt[irq], handler);
}

