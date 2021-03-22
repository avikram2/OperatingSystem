#include "interrupts.h"


//array for scancode keyboard inputs and characters to print
static char scan_code_array[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '0', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '0', '0', '0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '0', '0', '/'};

static char* video_mem = (char *)VIDEO;


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

//RTC interrupt handler. For checkpoint one, sufficient to call test_interrupts function.
// Reading from Register C to allow further interrupts also in test_interrupts function
//input/output: none 
//effect: invokes test interrupts to test RTC interrupt
void rtc_handler(){
    test_interrupts(); //call test_interrupts
}


