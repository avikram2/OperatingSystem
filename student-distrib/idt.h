/* idt.h - Defines idt handlers 
 * vim:ts=4 noexpandtab
 */

#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "lib.h"
#include "exceptions.h"
#include "interrupt_linkage.h"


/*Loads each idt entry to it's corresponding handler*/
extern void load_idt();

/* Sets up the corresponding exception idt entry and points it toward the given handler */
void set_exception_irq(int irq, void * handler);


//Sets up corresponding entry in idt table and points it to the handler
void set_interrupt_irq(int irq, void * handler);

//Sets up corresponding entry in idt table and points it to the handler
void set_system_call(int irq, void *handler);


//preliminary skeleton for handling a system call
void system_call_skeleton();

#endif //_IDT_H