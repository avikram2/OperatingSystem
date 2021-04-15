/* exceptions.c - Implements the various idt exception handlers
 * vim:ts=4 noexpandtab
 */

#include "exceptions.h"

//divide exception: handler to acknowledge divide by 0
void divide_exception()
{
    exception_halt();
}

//debug exception handler
void debug_exception()
{
    exception_halt();
}

//NMI exception handler 
void nmi_exception()
{
    exception_halt();
}

//breakpoint exception handler 
void breakpoint_exception()
{
    exception_halt();
}

//overflow exception handler
void overflow_exception()
{
    exception_halt();
}

//bounds range exception handler
void bound_range_exception()
{
    exception_halt();
}

//invalid opcode exception handler 
void invalid_opcode_exception()
{
    exception_halt();
}

//device not available exception handler
void device_not_available_exception()
{
    exception_halt();
}

//double fault exception handler
void double_fault_exception()
{
    exception_halt();
}

//coprocessor_segment_overrun_exception handler
void coprocessor_segment_overrun_exception()
{
    exception_halt();
}

//invalid tss exception handler
void invalid_tss_exception()
{
    exception_halt();
}

//segment not present exception handler
void segment_not_present_exception()
{
    exception_halt();
}

void stack_fault_exception()
{
    exception_halt();
}

void general_protection_exception()
{
    exception_halt();
}

void page_fault_exception()
{
uint32_t cr0, cr2, cr3;
    __asm__ __volatile__ (
        "mov %%cr0, %%eax\n\t"
        "mov %%eax, %0\n\t"
        "mov %%cr2, %%eax\n\t"
        "mov %%eax, %1\n\t"
        "mov %%cr3, %%eax\n\t"
        "mov %%eax, %2\n\t"
    : "=m" (cr0), "=m" (cr2), "=m" (cr3)
    : /* no input */
    : "%rax"
    );
    exception_halt();
}

void floating_point_exception()
{
    exception_halt();
}

void alignment_check_exception()
{
    exception_halt();
}

void machine_check_exception()
{
    exception_halt();
}

void simd_floating_point_exception()
{
    exception_halt();
}

void virtualization_exception()
{
    exception_halt();
}

void control_protection_exception()
{
    exception_halt();
}
