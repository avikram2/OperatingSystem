#include "interrupts.h"

//macro for the scancode of letter L
#define L_CODE 0x26
#define C_CODE 0x2E

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

static int alt_flag = 0; //track if alt is pressed

static int scheduling_count = 0;
// interrupt keyboard handler:
// will handle input from the keyboard when interrupt is generated and echo them to the screen
//inputs: none
//output: none
//effect: echo keyboard input to the screen, handle the interrupt
void interrupt_keyboard_handler(uint32_t jump_location,uint32_t esp_location){
    cli(); //disable interrupts
    unsigned int scancode;
    scancode = inb(DATA_PORT_KEYBOARD); //read in from the keyboard data port, 0x60
    //printf("scancode: %d\n", scancode);
    //Check if the scancode is a special key (shift, ctrl,... etc.) and set flags accordingly
    switch(scancode&SCAN_MASK){ //check which special char is pressed
      case LEFT_SHIFT: //if left shift is pressed
        shift_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
      case RIGHT_SHIFT: //if right shift is pressed
        shift_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
      case CAPS_LOCK: // if caps is pressed
        caps_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
	    case LEFT_CRTL: //if left control is pressed
        ctrl_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
      case LEFT_ALT: //if left-alt is pressed
        alt_flag = ((scancode&BREAK_MASK)==0);
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
    }

	if (scancode == SPACE){ //if the SPACE BAR is pressed
		typec(' '); //put space character
		update_cursor(get_cursor_x(), get_cursor_y()); //update cursor position
		if (terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] <= (BUFFER_SIZE-2)){ //add space to the buffer, as long as its not in the last position or outside
			terminal_info.keyboard_buffers[terminal_info.display_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]] = ' ';
			terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]++; //increment index of buffer
		}
	}
  else if (alt_flag){
    switch(scancode){
      case F1:
        send_eoi(KEYBOARD_IRQ_1);
        terminal_swap(0,esp_location,jump_location);
	sti();
        return;
      case F2:
        send_eoi(KEYBOARD_IRQ_1);
        terminal_swap(1,esp_location,jump_location);
	sti();
        return;
      case F3:
        send_eoi(KEYBOARD_IRQ_1);
        terminal_swap(2,esp_location,jump_location);
	sti();
        return;
    }
  }
  else if (scancode > SCAN_CODE_SIZE || scancode == ZERO_CODE || scancode == ESCAPE){ //if scancode is outside array, invalid, or escape key, then end interrupt
    send_eoi(KEYBOARD_IRQ_1);
    sti();
    return;
  }
	else if (scancode == ENTER){
	 //check if enter (newline, pressed)
	if (terminal_read_flag == ENABLE){//if the terminal_read_function is being invoked
  	if (terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] <= (BUFFER_SIZE-1)){ //if the enter is in the range of the buffer
  		terminal_info.keyboard_buffers[terminal_info.display_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]] = '\n'; //add to buffer
  		terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]+=1; //increment keyboard index after newline character
  	}
	}
	else {
  	typec(scan_code_default[scancode-1]); //write out newline to screen
  	update_cursor(get_cursor_x(), get_cursor_y()); //update cursor position
  	terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] = 0;
  	}
	}
	else if(scancode == BACK_SPACE){ //check if backspace pressed
      if(terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] == 0){//Check if there is no text to delete
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return; //Do not delete shell text, just return
      }
    	terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] = terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] - 1; //decrement index in buffer

    	int x_curr = get_cursor_x();
    	int y_curr = get_cursor_y();
    	if (x_curr == 0 && y_curr != 0){ //need to go back to prev line
      	x_curr = NUM_COLS-1; //last character in prev line
      	y_curr-=1; //back to prev line
      	update_cursor(x_curr, y_curr);
      	typec(' ');
      	//x_curr-=1; //previous position
      	update_cursor(x_curr, y_curr);
  	   }
  	else if (x_curr == 0 && y_curr == 0){
    	update_cursor(ORIGIN_CURSOR, ORIGIN_CURSOR);
    	//dont need to do anything when the cursor is at the beginning
  	}
  	else { //else if cursor was not at the beginning of a line
    	update_cursor(x_curr-1, y_curr);
    	typec(' '); //put the blank space, deleting previous charavter
      update_cursor(x_curr-1, y_curr);
  	}
	}
	else if(ctrl_flag&&(scancode==L_CODE)){ //Ctrl-L handle
      clear(); //clear the screen
	  //update_cursor(ORIGIN_CURSOR,ORIGIN_CURSOR); //send the cursor back to the beginning of the screen
	}
  else if(ctrl_flag&&(scancode==C_CODE)){
    send_eoi(KEYBOARD_IRQ_1);
    syscall_halt(0);
    sti();
  }
  else {
      if (shift_flag) { //check for shift held down
        if (caps_flag) { //check for caps lock
    		  if (terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] <= (BUFFER_SIZE-1)){ //add to the buffer, keeping space for newline
    			  terminal_info.keyboard_buffers[terminal_info.display_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]] = scan_code_caps_shift[scancode-1];
    			  terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]++;
            typec(scan_code_caps_shift[scancode-1]); //put character onto screen from the array
    		  }
        } else {  //only shift held
    		   if (terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] <= (BUFFER_SIZE-1)){ //add to the buffer, keeping space for newline
    			  terminal_info.keyboard_buffers[terminal_info.display_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]] = scan_code_shift[scancode-1];
    			  terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]++;
            typec(scan_code_shift[scancode-1]); //put character onto screen from the array
		        }
          }
      } else if(caps_flag){  //only caps held
      		if (terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] <= (BUFFER_SIZE-1)){ //add to the buffer, keeping space for newline
      			  terminal_info.keyboard_buffers[terminal_info.display_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]] = scan_code_caps[scancode-1];
      			  terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]++;
              typec(scan_code_caps[scancode-1]); //put character onto screen from the array
      		  }
      } else {
      		if (terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal] <= (BUFFER_SIZE-1)){ //add character to the buffer, keeping space for the last newline
      			terminal_info.keyboard_buffers[terminal_info.display_terminal][terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]] = scan_code_default[scancode-1];
      			terminal_info.keyboard_buffer_indexes[terminal_info.display_terminal]++; //increment current index in buffer
            typec(scan_code_default[scancode-1]); //put character onto screen from the array
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
void rtc_handler(uint32_t jump_location,uint32_t esp_location){

	int count = 0;

	cli();

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
	
	handle_possible_scheduling(jump_location,esp_location);
	sti();
}

void handle_possible_scheduling(uint32_t jump_location,uint32_t esp_location)
{
	cli();
    scheduling_count++;
    if(scheduling_count >= SCHEDULING_COUNT)
    {
       scheduling_count = 0;
    	terminal_info.esp_locations[terminal_info.current_terminal] = esp_location;
    	terminal_info.jump_locations[terminal_info.current_terminal] = jump_location;
       	uint32_t term = (terminal_info.current_terminal+1)%TERMINAL_NUMBER;
	while(terminal_info.active_terminals[term] == 0)
	{
		term = (term+1)%TERMINAL_NUMBER;
	}
	if(term != terminal_info.current_terminal)
	{
  		terminal_info.current_terminal = term;
	
       		switch_process(terminal_info.current_terminal);
	}
    }
	sti();
    
}
