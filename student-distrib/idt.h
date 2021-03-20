/* idt.h - Defines idt handlers 
 * vim:ts=4 noexpandtab
 */
#include "x86_desc.h"
#include "lib.h"
#include "exceptions.h"

/*Loads each idt entry to it's corresponding handler*/
extern void load_idt();

/* Sets up the corresponding exception idt entry and points it toward the given handler */
void set_exception_irq(int irq, void * handler);
