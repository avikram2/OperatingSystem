#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt.h"
#include "rtc_driver.h"
#include "terminal.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
  
	return result;
}

/* IDT Exceptions Test
 * 
 * Triggers an exception for user to verify
 * Inputs: None
 * Outputs: PASS
 * Side Effects: Show exception on screen
 * Coverage: IDT definition
 * Files: idt.h/c
 */
int idt_exceptions_test(){
	TEST_HEADER;
        
	asm volatile("int $0");
        
	asm volatile("int $5");
        
	asm volatile("int $9");

	return PASS;
}

// /* RTC Test
//  * 
//  * Sets the RTC handler to the test handler
//  * Inputs: None
//  * Outputs: PASS
//  * Side Effects: Oscillates characters on the screen
//  * Coverage: RTC InterruptHandler definition
//  * Files: idt.h/c, rtc.h/c
//  */
// int rtc_interrupt_test(){
// 	TEST_HEADER;
//    	set_interrupt_irq(0x28, test_interrupts);
//     	enable_irq(RTC_IRQ_NUM); //enable RTC interrupts


//    	int i = 0;
//     	while(i < FREEZE_LOOP_LENGTH){ i++; }
//     	disable_irq(RTC_IRQ_NUM); //enable RTC interrupts

// 	clear();
// 	return PASS;
// }

// add more tests here


//dereferencing_NULLPTR_test
//test to see if dereferencing a NULL pointer will raise the adaquete exception
//Covers: Exception Handling, Paging
int dereferencing_NULLPTR_test(){
	TEST_HEADER;
	int* x = NULL;
	int y;
	y = *x;
	return PASS;
}
//page fault test
//test to see if accessing a virtual memory location not mapped to a physical memory location will cause a page fault
//Covers: paging, exceptions
//files: exception.c, paging.c
int page_fault_test(){
	TEST_HEADER;
	int *x;
	x = (int*)(0x900000);
	(*x) = 5;
	return PASS;
}

//testing to see if accesses in video memory are permitted, in order to test if video memory paging set up properly
//covers paging, video memory paging
//files: paging.c
int video_memory_access_test(){
	TEST_HEADER;
	int *x;
	int y;
	x = (int*)(0xb8005);
	y = (*x);
	return PASS;
}

<<<<<<< student-distrib/tests.c
//testing to see if we can egenrate interrupts with the rtc driver
//covers rtc interrupts, the rtc driver
//should wait for 10 seconds, regardless of the frequency set
//files: rtc_driver.c/h
int rtc_test(){
	int frequency = 2;
	int count = 0;
	int rtc_instance;
	rtc_open(&rtc_instance);
	rtc_write(rtc_instance, frequency);
	for(count = 0;count<10 * frequency;count++)
=======
//testing to see if accesses in video memory are permitted, in order to test if video memory paging set up properly
//covers paging, video memory paging
//files: paging.c


/* Checkpoint 2 tests */
//RTC Test, to test the RTC_DRIVER
//covers the RTC Driver test, such as open, write, read and close functions
//file: rtc_driver.c/h
int rtc_test(){
	TEST_HEADER;
	int count = 0; //init count
	int rtc_instance;
	rtc_open(&rtc_instance);
	rtc_write(rtc_instance, 2);
	for(count = 0;count<20;count++) //run for twenty times
>>>>>>> student-distrib/tests.c
	{
		rtc_read(rtc_instance);
	}
	rtc_close(rtc_instance);
	return PASS;
}

int terminal_driver_test(){
	TEST_HEADER;

	terminal_open(NULL); //open terminal
	printf("Hello, what is your name ?");
	uint8_t buff[BUFFER_SIZE];
	int retval = terminal_read(0, buff, BUFFER_SIZE);
	printf("Hi, ");
	terminal_write(0, buff, retval);
}


/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("idt_exceptions_test", idt_exceptions_test());
	//TEST_OUTPUT("dereferencing_NULLPTR_test", dereferencing_NULLPTR_test());
	//TEST_OUTPUT("page fault", page_fault_test());
	//TEST_OUTPUT("video memory access", video_memory_access_test());
	TEST_OUTPUT("rtc_test", rtc_test());

	// launch your tests here
}
