/* kernel.c - the C part of the kernel
 * vim:ts=4 noexpandtab
 */

#include "idt.h"

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
}

void set_exception_irq(int irq, void * handler)
{
    idt[irq].size = 1;
    idt[irq].dpl = 0;
    idt[irq].seg_selector = 16;
    idt[irq].present = 1;
    idt[irq].reserved4 = 0;
    idt[irq].reserved3 = 1;
    idt[irq].reserved3 = 0xFFFF;
    idt[irq].reserved2 = 0xFFFF;
    idt[irq].reserved1 = 0xFFFF;
    SET_IDT_ENTRY(idt[irq],handler);
}

void divide_exception()
{
    clear();
    printf("Exception: You tried to divide by zero!\n");
}

void debug_exception()
{
    clear();
    printf("Exception: Debug condition!\n");
}

void nmi_exception()
{
    clear();
    printf("Exception: NMI triggered!\n");
}

void breakpoint_exception()
{
    clear();
    printf("Exception: Breakpoint called!\n");
}

void overflow_exception()
{
    clear();
    printf("Exception: Overflow!\n");
}

void bound_range_exception()
{
    clear();
    printf("Exception: You went outside of an array's bounds!\n");
}

void invalid_opcode_exception()
{
    clear();
    printf("Exception: You tried to use an invalid opcode!\n");
}

void device_not_available_exception()
{
    clear();
    printf("Exception: Device not available!\n");
}

void double_fault_exception()
{
    clear();
    printf("Exception: Double fault exception!\n");
}

void coprocessor_segment_overrun_exception()
{
    clear();
    printf("Exception: Coprocessor segment overrun!\n");
}

void invalid_tss_exception()
{
    clear();
    printf("Exception: TSS error!\n");
}

void segment_not_present_exception()
{
    clear();
    printf("Exception: Segment not present!\n");
}

void stack_fault_exception()
{
    clear();
    printf("Exception: Stack fault!\n");
}

void general_protection_exception()
{
    clear();
    printf("Exception: General protection!\n");
}

void page_fault_exception()
{
    clear();
    printf("Exception: Page fault!\n");
}

void floating_point_exception()
{
    clear();
    printf("Exception: Floating point!\n");
}

void alignment_check_exception()
{
    clear();
    printf("Exception: Alignment check!\n");
}

void machine_check_exception()
{
    clear();
    printf("Exception: Machine check!\n");
}

void simd_floating_point_exception()
{
    clear();
    printf("Exception: SIMD floating point!\n");
}

void virtualization_exception()
{
    clear();
    printf("Exception: Virtualizationo!\n");
}

void control_protection_exception()
{
    clear();
    printf("Exception: Control protection!\n");
}