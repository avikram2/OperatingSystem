/* exceptions.h - Defines the idt exception handlers
 * vim:ts=4 noexpandtab
 */

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include "lib.h"
#include "processes.h"
#define FREEZE_LOOP_LENGTH 1000000000 //length of loop in exception handler 


/* The various exception handlers for the idt to call */
void divide_exception();

void debug_exception();

void nmi_exception();

void breakpoint_exception();

void overflow_exception();

void bound_range_exception();

void invalid_opcode_exception();

void device_not_available_exception();

void double_fault_exception();

void coprocessor_segment_overrun_exception();

void invalid_tss_exception();

void segment_not_present_exception();

void stack_fault_exception();

void general_protection_exception();

void page_fault_exception();

void floating_point_exception();

void alignment_check_exception();

void machine_check_exception();

void simd_floating_point_exception();

void virtualization_exception();

void control_protection_exception();

#endif //_EXCEPTIONS_H
