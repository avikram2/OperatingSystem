#include "interrupts.h"


//array for scancode keyboard inputs and characters to print
static char scan_code_array[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '0', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '0', '0', '0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '0', '0', '/'};

//static char* video_mem = (char *)VIDEO;

static int rtc_counter = 0;

// interrupt keyboard handler:
// will handle input from the keyboard when interrupt is generated and echo them to the screen
//inputs: none
//output: none
//effect: echo keyboard input to the screen, handle the interrupt
void interrupt_keyboard_handler(){
    cli(); //disable interrupts
    unsigned int scancode; 
    scancode = inb(DATA_PORT_KEYBOARD); //read in from the keyboard data port, 0x60
    if (scancode >= SCAN_CODE_SIZE || scancode == 0){ //if scancode is outside array, then end interrupt
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
    }

    else {
        putc(scan_code_array[scancode-1]); //put character onto screen from the array
    }

    send_eoi(KEYBOARD_IRQ_1);
    sti(); //send eoi and enable interrupts
}

//RTC interrupt handler. Checks through each rtc driver instance and triggers it if it needs
//to be triggered
//input/output: none 
//effect: triggers rtc driver interrupts if needed
void rtc_handler(){

	int count = 0;
	
	//cli();

	//increment the rtc ticks counter
	rtc_counter++;
	//To prevent an eventual overflow, this value only needs to count up to 1024
	if(rtc_counter >= MAX_RTC_FREQUENCY)
	{
		rtc_counter = rtc_counter % MAX_RTC_FREQUENCY;
	}
	
	//iterate through each rtc driver instance and check if it should be triggered
	for(count = 0; count < MAX_RTC_DRIVERS;count++)
	{
		if(rtc_drivers_instances[count].wait_ticks != 0 && rtc_counter % rtc_drivers_instances[count].wait_ticks == 0)
		{
			rtc_drivers_instances[count].flag = 0;
		}
	}

    	outb(REGISTER_C, RTC_PORT);
    	inb(CMOS_PORT); // reading value from register C to ensure RTC interrupts happen again

    	send_eoi(RTC_IRQ_NUM); //send eoi to correct IRQ line 

	//sti();
}


