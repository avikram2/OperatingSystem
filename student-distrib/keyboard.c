
#include "keyboard.h"



//keyboard init:
//function to initialize the keyboard, enabling its interrupt line
//inputs/outputs: none
//effect: keyboard IRQ line (1) enabled
void keyboard_init(){
    enable_IRQ(KEYBOARD_IRQ_LINE); //enable keyboard interrupts 
}
