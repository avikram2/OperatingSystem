#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt.h"
#include "rtc_driver.h"
#include "terminal.h"
#include "filesystem.h"

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
	{
		rtc_read(rtc_instance);
	}
	rtc_close(rtc_instance);
	return PASS;
}

int terminal_driver_test(){
	TEST_HEADER;
	terminal_open(NULL); //open terminal
	clear();
	update_cursor(ORIGIN_CURSOR, ORIGIN_CURSOR);
	int8_t* string = "Hello, what is your name \n";
	uint8_t *unsigned_str = (uint8_t*) (string);
	// uint8_t * nullbuf = "\0\0\0\0\0a";
	// int j;
	// for (j = 0; j < 6; ++j){
	// 	putc(nullbuf[j])
	// }
	terminal_write(0, unsigned_str, strlen(string));
	uint8_t buffer_test[BUFFER_SIZE];
	int32_t retval = terminal_read(0, buffer_test, BUFFER_SIZE-1);
	string = "Hi, ";
	unsigned_str = (uint8_t*) (string);
	terminal_write(0, unsigned_str, (int32_t)(strlen(string)));
	terminal_write(0, buffer_test, retval);
	terminal_close(0);
	return PASS;
}

int echo_terminal_test(){
	TEST_HEADER;
	terminal_open(NULL);
	clear();
	update_cursor(ORIGIN_CURSOR, ORIGIN_CURSOR);
	while (1){
		uint8_t buffer_test[BUFFER_SIZE];
		int retval = terminal_read(0, buffer_test, BUFFER_SIZE);
		terminal_write(0, buffer_test, retval);
	}
	return PASS;
}


int read_data_test(){
	uint8_t buf[FS_BUF_LENGTH];
	
	//SET FILE NAME
	uint8_t fname[MAX_NAME_LENGTH] = "frame1.txt";
	dentry_t temp;
	uint32_t file_len, bytes_read;
	
	read_dentry_by_name(fname, &temp);
	inode_t* inode = (inode_t*)((uint32_t)file_sys + (temp.inode_idx + 1) * SIZE_BLOCK);
	file_len = inode->length;
	bytes_read = read_data(temp.inode_idx, 0, buf, file_len);
	
	
	printf("file_read(%s)\n", fname); // testing file_read
	
	
	//test read_dentry_by_name output
	//test needs to get redefined to match entire dentry to requested dentry
	printf("file name: ");
	uint32_t i;
	for (i = 0; i < MAX_NAME_LENGTH; i++) {
		printf("%c", temp.filename[i]);
	}
	printf("\n");
	if(strncmp((char*)fname, (char*)temp.filename, MAX_NAME_LENGTH) != 0)
		return FAIL;
	
	printf("read_dentry_by_name(): PASS\n");
	
	//test read_data
	printf("file length: %d\n", bytes_read);
	if(bytes_read == -1)
		return FAIL;
	
	
	//Uncomment below to print file data
	printf("file data: \n");
	printf("%s", (char*)buf);
	
	while(1);
	return PASS;
}

int file_read_test(){
	uint8_t buf[10];
	uint32_t position = 0;
	uint32_t res;
	//SET FILE NAME
	uint8_t fname[MAX_NAME_LENGTH] = "frame1.txt";
	res = file_read(fname, buf, 10 - 1, &position);
	while(res != 0 && res != -1)
	{
		printf("%s", buf);
		res = file_read(fname, buf, 10 - 1, &position);
	}
	return PASS;
}

int directory_read_test(){
	uint8_t fname[MAX_NAME_LENGTH] = ".";
	uint8_t buf[MAX_NAME_LENGTH];
	uint32_t position = 0;
	while(directory_read(fname, buf, MAX_NAME_LENGTH, &position) != -1)
	{
		printf(buf);
		printf("\n");
	}
	return PASS;
}

int syscall_test(){
	int out;
        uint8_t file[3] = "ls";
	asm volatile("movl %1,%%ebx \n\t"
			"movl $1,%%eax \n\t"
			"int $0x80 \n\t"
			:"=r" (out)
			:"r" (&file)
			);
	return PASS;
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
	//TEST_OUTPUT("rtc_test", rtc_test());
	
	
	/* Checkpoint 2 Test */
	//TEST_OUTPUT("terminal_driver_test", terminal_driver_test());
	//TEST_OUTPUT("echo_terminal_test", echo_terminal_test());
	//TEST_OUTPUT("read_data test", read_data_test());
	//TEST_OUTPUT("read_data", file_read_test());
	//TEST_OUTPUT("read_directory", directory_read_test());

	TEST_OUTPUT("syscall", syscall_test());

	// launch your tests here
}
