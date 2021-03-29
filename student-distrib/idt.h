/* idt.h - Defines idt handlers 
 * vim:ts=4 noexpandtab
 */

#ifndef _IDT_H
#define _IDT_H


#include "interrupt_linkage.h"
#include "x86_desc.h"
#include "lib.h"
#include "exceptions.h"

#ifndef ASM

/*Loads each idt entry to it's corresponding handler*/
extern void load_idt();

/* Sets up the corresponding exception idt entry and points it toward the given handler */
void set_exception_irq(int irq, void * handler);


//Sets up corresponding entry in idt table and points it to the handler
extern void set_interrupt_irq(int irq, void * handler);

//Sets up corresponding entry in idt table and points it to the handler
void set_system_call(int irq, void *handler);


//preliminary skeleton for handling a system call
void system_call_skeleton();

//initialize the entry in the idt with the proper fields
void init_idt_entry(int irq);


#endif

#endif //_IDT_H

