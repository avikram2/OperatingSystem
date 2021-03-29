
#include "interrupts.h"

//macro for the scancode of letter L
#define L_CODE 0x26 

//array for scancode keyboard inputs and characters to print
static char scan_code_default[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', ' ', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '0', '0', '0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'};

static char scan_code_shift[SCAN_CODE_SIZE] = {
'0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '0', '0', '0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'};

static char scan_code_caps[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', '0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '0', '0', '0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/'};

static char scan_code_caps_shift[SCAN_CODE_SIZE] = {
'0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', ' ', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '0', '0', '0', '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?'};

//static char* video_mem = (char *)VIDEO;

//Flags for tracking special keys (1=>pressed, 0=>not pressed)
static int shift_flag = 0; //variable for tracking shift state

static int caps_flag = 0; //variable for tracking caps state

static int rtc_counter = 0; //ticks counter for RTC 

static int ctrl_flag = 0; //track if control is pressed

// interrupt keyboard handler:
// will handle input from the keyboard when interrupt is generated and echo them to the screen
//inputs: none
//output: none
//effect: echo keyboard input to the screen, handle the interrupt
void interrupt_keyboard_handler(){
    cli(); //disable interrupts
    unsigned int scancode;
    scancode = inb(DATA_PORT_KEYBOARD); //read in from the keyboard data port, 0x60
    //Check if the scancode is a special key (shift, ctrl,... etc.) and set flags accordingly
    switch(scancode&SCAN_MASK){
      case LEFT_SHIFT:
        shift_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
      case RIGHT_SHIFT:
        shift_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
      case CAPS_LOCK:
        caps_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
	case LEFT_CRTL:
        ctrl_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
    }

    if (scancode >= SCAN_CODE_SIZE || scancode == 0){ //if scancode is outside array, then end interrupt
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
    }

	else if (scancode == ENTER){
	 //check if enter (newline, pressed)
	if (keyboard_buffer_index <= BUFFER_SIZE-1){ //if the enter is in the range of the buffer
		keyboard_buffer[keyboard_buffer_index] = '\n';
		keyboard_buffer_index+=1; //increment keyboard index after newline character
		int num_bytes = terminal_read(0, buffer, keyboard_buffer_index); //call terminal read
		terminal_write(0, buffer, num_bytes); //call terminal write
	}
	putc(scan_code_default[scancode-1]); //write out newline to screen
	}
	else if(scancode == BACK_SPACE){ //check if backspace pressed

	keyboard_buffer_index = (keyboard_buffer_index == 0)? 0: (keyboard_buffer_index-1); //decrement index in buffer

	int x_curr = get_cursor_x();
	int y_curr = get_cursor_y();
	if (x_curr == 0 && y_curr != 0){ //need to go back to prev line 
	x_curr = NUM_COLS-1; //last character in prev line
	y_curr-=1; //back to prev line
	update_cursor(x_curr, y_curr);
	putc(' ');
	x_curr-=1; //previous position
	update_cursor(x_curr, y_curr);
	}
	else if (x_curr == 0 && y_curr == 0){
	update_cursor(ORIGIN_CURSOR, ORIGIN_CURSOR);
	//dont need to do anything when the cursor is at the beginning
	}
	else { //else if cursor was not at the beginning of a line
	update_cursor(x_curr-1, y_curr);
	putc(' ');
	x_curr -=1;
	if (x_curr == 0 && y_curr != 0){
	x_curr = NUM_COLS-1;
	y_curr -=1;
	update_cursor(x_curr, y_curr);
	}
	else if (x_curr == 0 && y_curr == 0){
	update_cursor(ORIGIN_CURSOR, ORIGIN_CURSOR);
	}
	else {
	update_cursor(x_curr, y_curr);
	}

	} 
	}
	else if(ctrl_flag&&(scancode==L_CODE)){ //Ctrl-L handle
      clear(); //clear the screen
	  update_cursor(ORIGIN_CURSOR,ORIGIN_CURSOR); //send the cursor back to the beginning of the screen
	}

    else {
      if (shift_flag) { //check for shift held down
        if (caps_flag) { //check for caps lock
          putc(scan_code_caps_shift[scancode-1]); //put character onto screen from the array
		  if (keyboard_buffer_index <= (BUFFER_SIZE-2)){ //add to the buffer, keeping space for newline
			  keyboard_buffer[keyboard_buffer_index] = scan_code_caps_shift[scancode-1];
			  keyboard_buffer_index++;
		  }
        } else {  //only shift held
          putc(scan_code_shift[scancode-1]); //put character onto screen from the array

		   if (keyboard_buffer_index <= (BUFFER_SIZE-2)){ //add to the buffer, keeping space for newline
			  keyboard_buffer[keyboard_buffer_index] = scan_code_shift[scancode-1];
			  keyboard_buffer_index++;
		  }
        }
      } else if(caps_flag){  //only caps held
        putc(scan_code_caps[scancode-1]); //put character onto screen from the array

		if (keyboard_buffer_index <= (BUFFER_SIZE-2)){ //add to the buffer, keeping space for newline
			  keyboard_buffer[keyboard_buffer_index] = scan_code_caps[scancode-1];
			  keyboard_buffer_index++;
		  }
      } else {
        putc(scan_code_default[scancode-1]); //put character onto screen from the array

		if (keyboard_buffer_index <= (BUFFER_SIZE-2)){ //add character to the buffer, keeping space for the last newline
			keyboard_buffer[keyboard_buffer_index] = scan_code_default[scancode-1];
			keyboard_buffer_index++; //increment current index in buffer
		}
	}
	update_cursor(get_cursor_x(), get_cursor_y()); //mode cursor position to reflect added character
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


