/* exceptions.c - Implements the various idt exception handlers
 * vim:ts=4 noexpandtab
 */

#include "idt.h"


void divide_exception()
{
    clear();
    printf("Exception: You tried to divide by zero!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void debug_exception()
{
    clear();
    printf("Exception: Debug condition!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void nmi_exception()
{
    clear();
    printf("Exception: NMI triggered!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void breakpoint_exception()
{
    clear();
    printf("Exception: Breakpoint called!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void overflow_exception()
{
    clear();
    printf("Exception: Overflow!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void bound_range_exception()
{
    clear();
    printf("Exception: You went outside of an array's bounds!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void invalid_opcode_exception()
{
    clear();
    printf("Exception: You tried to use an invalid opcode!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void device_not_available_exception()
{
    clear();
    printf("Exception: Device not available!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void double_fault_exception()
{
    clear();
    printf("Exception: Double fault exception!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void coprocessor_segment_overrun_exception()
{
    clear();
    printf("Exception: Coprocessor segment overrun!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void invalid_tss_exception()
{
    clear();
    printf("Exception: TSS error!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void segment_not_present_exception()
{
    clear();
    printf("Exception: Segment not present!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void stack_fault_exception()
{
    clear();
    printf("Exception: Stack fault!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void general_protection_exception()
{
    clear();
    printf("Exception: General protection!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void page_fault_exception()
{
    clear();
    printf("Exception: Page fault!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void floating_point_exception()
{
    clear();
    printf("Exception: Floating point!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void alignment_check_exception()
{
    clear();
    printf("Exception: Alignment check!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void machine_check_exception()
{
    clear();
    printf("Exception: Machine check!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void simd_floating_point_exception()
{
    clear();
    printf("Exception: SIMD floating point!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void virtualization_exception()
{
    clear();
    printf("Exception: Virtualizationo!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}

void control_protection_exception()
{
    clear();
    printf("Exception: Control protection!\n");

    //While loop to freeze the screen
    int i = 0;
    while(i < FREEZE_LOOP_LENGTH){ i++; }
}