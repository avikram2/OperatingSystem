#include "interrupts.h"


static char scan_code_array[SCAN_CODE_SIZE] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '0', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '0', '0', '0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '0', '0', '/'};

static char* video_mem = (char *)VIDEO;

void interrupt_keyboard_handler(){
    cli();
    unsigned int scancode;
    scancode = inb(DATA_PORT);
    if (scancode >= SCAN_CODE_SIZE){
        send_eoi(KEYBOARD_IRQ_1);
        sti();
        return;
    }

    else {
        putc(scan_code_array[scancode-1]);
    }

    send_eoi(KEYBOARD_IRQ_1);
    sti();
}


void rtc_handler(){
    cli();
    int32_t i;
    for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        video_mem[i << 1]++;
    }
    send_eoi(RTC_IRQ_NUM);
    sti();
}

