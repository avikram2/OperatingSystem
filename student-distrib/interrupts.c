#include "interrupts.h"


//array for scancode keyboard inputs and characters to print
static char scan_code_default[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '0', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '0', '0', '0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '0', '0', '/'};

static char scan_code_shift[SCAN_CODE_SIZE] = {
'0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '0', '0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '0', '0', '0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '0', '0', '?'};

static char scan_code_caps[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '0', '0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', '0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '0', '0', '0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '0', '0', '/'};

static char scan_code_caps_shift[SCAN_CODE_SIZE] = {
'0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '0', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '0', '0', '0', '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '0', '0', '?'};

//static char* video_mem = (char *)VIDEO;

//Flags for tracking special keys (1=>pressed, 0=>not pressed)
static int shift_flag = 0; //variable for tracking shift state

static int caps_flag = 0; //variable for tracking caps state

static int ctrl_flag = 0; //varaible for tracking ctrl state

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

    // Still need to replace putc statements with char buffer to be used in the terminal
    if (scancode >= SCAN_CODE_SIZE || scancode == 0){ //if scancode is outside array, then end interrupt
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
    }
    else if(ctrl_flag&&(scancode=="l")){ //Ctrl-L handle
      int i;
      for(i=0; i < 100; i++){
        putc("\n"); //rudimentary way of clearing the screen for now
      }
    } else { //remaining cases
      if (shift_flag) { //check for shift held down
        if (caps_flag) { //check for caps lock
          putc(scan_code_caps_shift[scancode-1]); //put character onto screen from the array
        } else {  //only shift held
          putc(scan_code_shift[scancode-1]); //put character onto screen from the array
        }
      } else if(caps_flag){  //only caps held
        putc(scan_code_caps[scancode-1]); //put character onto screen from the array
      } else {
        putc(scan_code_default[scancode-1]); //put character onto screen from the array
      }
    }

    send_eoi(KEYBOARD_IRQ_1);
    sti(); //send eoi and enable interrupts
}

//RTC interrupt handler. For checkpoint one, sufficient to call test_interrupts function.
// Reading from Register C to allow further interrupts also in test_interrupts function
//input/output: none
//effect: invokes test interrupts to test RTC interrupt
void rtc_handler(){
    test_interrupts(); //call test_interrupts
}
