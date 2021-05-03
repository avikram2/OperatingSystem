/* lib.c - Some basic library functions (printf, strlen, etc.)
 * vim:ts=4 noexpandtab */

#include "lib.h"

#define NUM_COLS    80
#define NUM_ROWS    25
#define ATTRIB      0x7

//macros for the vga ports to move the cursor
#define VGA_PORT_1 0x3D4
#define VGA_PORT_2 0x3D5

static int screen_x[3] = {ORIGIN_CURSOR,ORIGIN_CURSOR,ORIGIN_CURSOR};
static int screen_y[3] = {ORIGIN_CURSOR,ORIGIN_CURSOR,ORIGIN_CURSOR};
static char* video_mem = (char *)VIDEO;

//custum variables
//vidmem buffer addresses
static char* buf0;
static char* buf1;
static char* buf2;
//terminal display/process execution
static int terminal_display;
static int terminal_process;
static int cursor_terminal;
//1/0 variable identifying if the process is matching or not
static int matching;
//begin custom functions

//set_buffers(char* b0, char* b1, char* b2)
//function to set the local addresses of the buffer
//inputs: 3 buffer addresses
//output: none
//side effect: initialize the 3 buffers for writing to when terminal not displayed
void set_buffers(uint8_t* b0, uint8_t* b1, uint8_t* b2){
  buf0 = b0;
  buf1 = b1;
  buf2 = b2;
}

//set_display(int term)
//function to set the current terminal being displayed
//inputs: current terminal displayed
//output: none
//side effect: set displayed terminal
void set_display(int term){
  terminal_display = term;
  //printf("d: %d\n", term);
  update_video_mem(); //update vid_mem pointer with new display
}

//set_process(int term)
//function to set the current terminal being executed
//inputs: current terminal being executed
//output: none
//side effect: set executed terminal
void set_process(int term){
  terminal_process = term;
  update_video_mem(); //update vid_mem pointer with new process
  //printf("p: %d\n", term);
}

void set_cursor_terminal(int term){
  if(!(matching)){//if the process not displayed is writing, change cursor tp buffer
    cursor_terminal = terminal_process;
  }else{//else cursor for the display
    cursor_terminal = terminal_display;
  }
}

//update_video_mem()
//function to set the video_mem pointer
//inputs: none
//output: none
//side effect: changes the video memory pointer
void update_video_mem(){
  //if process matches display, write to display
  if(terminal_display == terminal_process){
    video_mem = (char *)VIDEO;
    matching = 1;
  }else{
    //if process does not equal display, write to buffer of process
    matching = 0;
    switch(terminal_process){
      case 0:
        video_mem = buf0;
        return;
      case 1:
        video_mem = buf1;
        return;
      case 2:
        video_mem = buf2;
        return;
    }
  }
  set_cursor_terminal(terminal_process);
  update_cursor(screen_x[cursor_terminal], screen_y[cursor_terminal]);
  *(uint8_t *)(video_mem + ((NUM_COLS * screen_y[cursor_terminal] + screen_x[cursor_terminal]) << 1) + 1) = ATTRIB;
}

//update_cursor(int x, int y)
//function to update position of the cursor on the screen to a certain coordinate position
//inputs: (x,y)
//output: none
//side effect: changes cursor to point to new
void update_cursor(int x, int y){
    screen_x[cursor_terminal] = x;
    screen_y[cursor_terminal] = y; //set screen position
    uint16_t pos = screen_y[terminal_display] * NUM_COLS + screen_x[terminal_display]; //linearized position
    outb(VGA_CURSOR_MASK, VGA_PORT_1); //send relevant values to VGA registers
    outb((uint8_t)(pos & CURSOR_BITMASK), VGA_PORT_2);
    outb(VGA_MASK_2, VGA_PORT_1);
    outb((uint8_t)((pos >> 8) & CURSOR_BITMASK), VGA_PORT_2); //write VGA registers to get the visible cursor
}

//get_cursor_y
//function to obtain the 'y' position of the cursor
//inputs: none, outputs: the 'y' location of the cursor
//side effect: returns y coordinate of the cursor
int get_cursor_y(){
    return screen_y[cursor_terminal];
}

//get_cursor_x
//function to obtain the 'x' position of the cursor
//inputs: none, outputs: the 'x' location of the cursor
//side effect: returns x coordinate of the cursor
int get_cursor_x(){
    return screen_x[cursor_terminal];
}

/* void clear(void);
 * Inputs: void
 * Return Value: none
 * Function: Clears video memory */
void clear(void) {
    int32_t i;
    int x_curr = get_cursor_x();
    int y_curr = get_cursor_y();
    for(i = 0; i < y_curr; i++){
      scrolling();
    }
    update_cursor(x_curr, ORIGIN_CURSOR);
    /*
    for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
    }
    */
}


char* get_vid_mem(){
  return video_mem;
}

/* Standard printf().
 * Only supports the following format strings:
 * %%  - print a literal '%' character
 * %x  - print a number in hexadecimal
 * %u  - print a number as an unsigned integer
 * %d  - print a number as a signed integer
 * %c  - print a character
 * %s  - print a string
 * %#x - print a number in 32-bit aligned hexadecimal, i.e.
 *       print 8 hexadecimal digits, zero-padded on the left.
 *       For example, the hex number "E" would be printed as
 *       "0000000E".
 *       Note: This is slightly different than the libc specification
 *       for the "#" modifier (this implementation doesn't add a "0x" at
 *       the beginning), but I think it's more flexible this way.
 *       Also note: %x is the only conversion specifier that can use
 *       the "#" modifier to alter output. */
int32_t printf(int8_t *format, ...) {

    /* Pointer to the format string */
    int8_t* buf = format;

    /* Stack pointer for the other parameters */
    int32_t* esp = (void *)&format;
    esp++;

    while (*buf != '\0') {
        switch (*buf) {
            case '%':
                {
                    int32_t alternate = 0;
                    buf++;

format_char_switch:
                    /* Conversion specifiers */
                    switch (*buf) {
                        /* Print a literal '%' character */
                        case '%':
                            putc('%');
                            break;

                        /* Use alternate formatting */
                        case '#':
                            alternate = 1;
                            buf++;
                            /* Yes, I know gotos are bad.  This is the
                             * most elegant and general way to do this,
                             * IMHO. */
                            goto format_char_switch;

                        /* Print a number in hexadecimal form */
                        case 'x':
                            {
                                int8_t conv_buf[64];
                                if (alternate == 0) {
                                    itoa(*((uint32_t *)esp), conv_buf, 16);
                                    puts(conv_buf);
                                } else {
                                    int32_t starting_index;
                                    int32_t i;
                                    itoa(*((uint32_t *)esp), &conv_buf[8], 16);
                                    i = starting_index = strlen(&conv_buf[8]);
                                    while(i < 8) {
                                        conv_buf[i] = '0';
                                        i++;
                                    }
                                    puts(&conv_buf[starting_index]);
                                }
                                esp++;
                            }
                            break;

                        /* Print a number in unsigned int form */
                        case 'u':
                            {
                                int8_t conv_buf[36];
                                itoa(*((uint32_t *)esp), conv_buf, 10);
                                puts(conv_buf);
                                esp++;
                            }
                            break;

                        /* Print a number in signed int form */
                        case 'd':
                            {
                                int8_t conv_buf[36];
                                int32_t value = *((int32_t *)esp);
                                if(value < 0) {
                                    conv_buf[0] = '-';
                                    itoa(-value, &conv_buf[1], 10);
                                } else {
                                    itoa(value, conv_buf, 10);
                                }
                                puts(conv_buf);
                                esp++;
                            }
                            break;

                        /* Print a single character */
                        case 'c':
                            putc((uint8_t) *((int32_t *)esp));
                            esp++;
                            break;

                        /* Print a NULL-terminated string */
                        case 's':
                            puts(*((int8_t **)esp));
                            esp++;
                            break;

                        default:
                            break;
                    }

                }
                break;

            default:
                putc(*buf);
                break;
        }
        buf++;
    }
    return (buf - format);
}

/* int32_t puts(int8_t* s);
 *   Inputs: int_8* s = pointer to a string of characters
 *   Return Value: Number of bytes written
 *    Function: Output a string to the console */
int32_t puts(int8_t* s) {
    register int32_t index = 0;
    while (s[index] != '\0') {
        putc(s[index]);
        index++;
    }
    return index;
}

//scrolling
//performs scrolling by moving all of the lines up by one place and then producing a new empty last line
//input: nothing, output: nothing
//side effect: implements a scrolling effect so that when running out of screen space, the lines are shifted to make space
void scrolling(){
int second_line_offset = NUM_COLS*2;
	//the offset to the second line is the number of elements
	//in the first line * 2, since each character is 2 bytes
int number_of_bytes = (NUM_ROWS-1)*(NUM_COLS) + (NUM_ROWS-1)*(NUM_COLS);
//the number of bytes to be copied upwards.
//1 is subtracted from NUM_ROWS because of first row not counting
memmove(video_mem, video_mem + second_line_offset, number_of_bytes); //move memory
int i = 0;

while (i < NUM_COLS){ //for all the characters in the line, clear them, aka set to 'space' character and set color
*(uint8_t*)((video_mem + NUM_COLS * 2* (NUM_ROWS-1) + (i << 1))) = ' '; //multiplication by 2 due to the fact that two bytes = one character
*(uint8_t*)(video_mem+NUM_COLS*2*(NUM_ROWS-1) + (i << 1) + 1) = ATTRIB;
++i;
}
update_cursor(ORIGIN_CURSOR, NUM_ROWS-1); //set cursor to the first character of the new last line
screen_x[cursor_terminal] = ORIGIN_CURSOR;
screen_y[cursor_terminal] = NUM_ROWS-1;
}




/* void putc(uint8_t c);
 * Inputs: uint_8* c = character to print
 * Return Value: void
 *  Function: Output a character to the console */
void putc(uint8_t c) {
    if(c == '\n') {
        screen_y[cursor_terminal]++;
        screen_x[cursor_terminal] = ORIGIN_CURSOR; //first position of newline
		if (screen_y[cursor_terminal] >= NUM_ROWS){ //if leaving the screen, then implement scrolling
			scrolling();
		}

    }

	else if(c == '\r')
		screen_x[cursor_terminal] = ORIGIN_CURSOR;


	else {
        *(uint8_t *)(video_mem + ((NUM_COLS * screen_y[cursor_terminal] + screen_x[cursor_terminal]) << 1)) = c;
        *(uint8_t *)(video_mem + ((NUM_COLS * screen_y[cursor_terminal] + screen_x[cursor_terminal]) << 1) + 1) = ATTRIB;
        screen_x[cursor_terminal]++; //increment horiz position
        screen_y[cursor_terminal] = (screen_y[cursor_terminal] + (screen_x[cursor_terminal] / NUM_COLS)); //if necessary new line
	    screen_x[cursor_terminal] = screen_x[cursor_terminal] % NUM_COLS; //adjust overflow of horiz position
		if (screen_y[cursor_terminal] >= NUM_ROWS){ //implement scrolling if necessary
			scrolling();
		}
    }
}

/* void typec(uint8_t c);
 * Inputs: uint_8* c = character to print
 * Return Value: void
 *  Function: Output a character to the console */
void typec(uint8_t c) {
    if(c == '\n') {
        screen_y[terminal_display]++;
        screen_x[terminal_display] = ORIGIN_CURSOR; //first position of newline
		if (screen_y[terminal_display] >= NUM_ROWS){ //if leaving the screen, then implement scrolling
			scrolling();
		}

    }

	else if(c == '\r')
		screen_x[terminal_display] = ORIGIN_CURSOR;


	else {
        *(uint8_t *)((char *)VIDEO + ((NUM_COLS * screen_y[terminal_display] + screen_x[terminal_display]) << 1)) = c;
        *(uint8_t *)((char *)VIDEO + ((NUM_COLS * screen_y[terminal_display] + screen_x[terminal_display]) << 1) + 1) = ATTRIB;
        screen_x[terminal_display]++; //increment horiz position
        screen_y[terminal_display] = (screen_y[terminal_display] + (screen_x[terminal_display] / NUM_COLS)); //if necessary new line
	    screen_x[terminal_display] = screen_x[terminal_display] % NUM_COLS; //adjust overflow of horiz position
		if (screen_y[terminal_display] >= NUM_ROWS){ //implement scrolling if necessary
			scrolling();
		}
    }
}

/* int8_t* itoa(uint32_t value, int8_t* buf, int32_t radix);
 * Inputs: uint32_t value = number to convert
 *            int8_t* buf = allocated buffer to place string in
 *          int32_t radix = base system. hex, oct, dec, etc.
 * Return Value: number of bytes written
 * Function: Convert a number to its ASCII representation, with base "radix" */
int8_t* itoa(uint32_t value, int8_t* buf, int32_t radix) {
    static int8_t lookup[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int8_t *newbuf = buf;
    int32_t i;
    uint32_t newval = value;

    /* Special case for zero */
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    /* Go through the number one place value at a time, and add the
     * correct digit to "newbuf".  We actually add characters to the
     * ASCII string from lowest place value to highest, which is the
     * opposite of how the number should be printed.  We'll reverse the
     * characters later. */
    while (newval > 0) {
        i = newval % radix;
        *newbuf = lookup[i];
        newbuf++;
        newval /= radix;
    }

    /* Add a terminating NULL */
    *newbuf = '\0';

    /* Reverse the string and return */
    return strrev(buf);
}

/* int8_t* strrev(int8_t* s);
 * Inputs: int8_t* s = string to reverse
 * Return Value: reversed string
 * Function: reverses a string s */
int8_t* strrev(int8_t* s) {
    register int8_t tmp;
    register int32_t beg = 0;
    register int32_t end = strlen(s) - 1;

    while (beg < end) {
        tmp = s[end];
        s[end] = s[beg];
        s[beg] = tmp;
        beg++;
        end--;
    }
    return s;
}

/* uint32_t strlen(const int8_t* s);
 * Inputs: const int8_t* s = string to take length of
 * Return Value: length of string s
 * Function: return length of string s */
uint32_t strlen(const int8_t* s) {
    register uint32_t len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

/* void* memset(void* s, int32_t c, uint32_t n);
 * Inputs:    void* s = pointer to memory
 *          int32_t c = value to set memory to
 *         uint32_t n = number of bytes to set
 * Return Value: new string
 * Function: set n consecutive bytes of pointer s to value c */
void* memset(void* s, int32_t c, uint32_t n) {
    c &= 0xFF;
    asm volatile ("                 \n\
            .memset_top:            \n\
            testl   %%ecx, %%ecx    \n\
            jz      .memset_done    \n\
            testl   $0x3, %%edi     \n\
            jz      .memset_aligned \n\
            movb    %%al, (%%edi)   \n\
            addl    $1, %%edi       \n\
            subl    $1, %%ecx       \n\
            jmp     .memset_top     \n\
            .memset_aligned:        \n\
            movw    %%ds, %%dx      \n\
            movw    %%dx, %%es      \n\
            movl    %%ecx, %%edx    \n\
            shrl    $2, %%ecx       \n\
            andl    $0x3, %%edx     \n\
            cld                     \n\
            rep     stosl           \n\
            .memset_bottom:         \n\
            testl   %%edx, %%edx    \n\
            jz      .memset_done    \n\
            movb    %%al, (%%edi)   \n\
            addl    $1, %%edi       \n\
            subl    $1, %%edx       \n\
            jmp     .memset_bottom  \n\
            .memset_done:           \n\
            "
            :
            : "a"(c << 24 | c << 16 | c << 8 | c), "D"(s), "c"(n)
            : "edx", "memory", "cc"
    );
    return s;
}

/* void* memset_word(void* s, int32_t c, uint32_t n);
 * Description: Optimized memset_word
 * Inputs:    void* s = pointer to memory
 *          int32_t c = value to set memory to
 *         uint32_t n = number of bytes to set
 * Return Value: new string
 * Function: set lower 16 bits of n consecutive memory locations of pointer s to value c */
void* memset_word(void* s, int32_t c, uint32_t n) {
    asm volatile ("                 \n\
            movw    %%ds, %%dx      \n\
            movw    %%dx, %%es      \n\
            cld                     \n\
            rep     stosw           \n\
            "
            :
            : "a"(c), "D"(s), "c"(n)
            : "edx", "memory", "cc"
    );
    return s;
}

/* void* memset_dword(void* s, int32_t c, uint32_t n);
 * Inputs:    void* s = pointer to memory
 *          int32_t c = value to set memory to
 *         uint32_t n = number of bytes to set
 * Return Value: new string
 * Function: set n consecutive memory locations of pointer s to value c */
void* memset_dword(void* s, int32_t c, uint32_t n) {
    asm volatile ("                 \n\
            movw    %%ds, %%dx      \n\
            movw    %%dx, %%es      \n\
            cld                     \n\
            rep     stosl           \n\
            "
            :
            : "a"(c), "D"(s), "c"(n)
            : "edx", "memory", "cc"
    );
    return s;
}

/* void* memcpy(void* dest, const void* src, uint32_t n);
 * Inputs:      void* dest = destination of copy
 *         const void* src = source of copy
 *              uint32_t n = number of byets to copy
 * Return Value: pointer to dest
 * Function: copy n bytes of src to dest */
void* memcpy(void* dest, const void* src, uint32_t n) {
    asm volatile ("                 \n\
            .memcpy_top:            \n\
            testl   %%ecx, %%ecx    \n\
            jz      .memcpy_done    \n\
            testl   $0x3, %%edi     \n\
            jz      .memcpy_aligned \n\
            movb    (%%esi), %%al   \n\
            movb    %%al, (%%edi)   \n\
            addl    $1, %%edi       \n\
            addl    $1, %%esi       \n\
            subl    $1, %%ecx       \n\
            jmp     .memcpy_top     \n\
            .memcpy_aligned:        \n\
            movw    %%ds, %%dx      \n\
            movw    %%dx, %%es      \n\
            movl    %%ecx, %%edx    \n\
            shrl    $2, %%ecx       \n\
            andl    $0x3, %%edx     \n\
            cld                     \n\
            rep     movsl           \n\
            .memcpy_bottom:         \n\
            testl   %%edx, %%edx    \n\
            jz      .memcpy_done    \n\
            movb    (%%esi), %%al   \n\
            movb    %%al, (%%edi)   \n\
            addl    $1, %%edi       \n\
            addl    $1, %%esi       \n\
            subl    $1, %%edx       \n\
            jmp     .memcpy_bottom  \n\
            .memcpy_done:           \n\
            "
            :
            : "S"(src), "D"(dest), "c"(n)
            : "eax", "edx", "memory", "cc"
    );
    return dest;
}

/* void* memmove(void* dest, const void* src, uint32_t n);
 * Description: Optimized memmove (used for overlapping memory areas)
 * Inputs:      void* dest = destination of move
 *         const void* src = source of move
 *              uint32_t n = number of byets to move
 * Return Value: pointer to dest
 * Function: move n bytes of src to dest */
void* memmove(void* dest, const void* src, uint32_t n) {
    asm volatile ("                             \n\
            movw    %%ds, %%dx                  \n\
            movw    %%dx, %%es                  \n\
            cld                                 \n\
            cmp     %%edi, %%esi                \n\
            jae     .memmove_go                 \n\
            leal    -1(%%esi, %%ecx), %%esi     \n\
            leal    -1(%%edi, %%ecx), %%edi     \n\
            std                                 \n\
            .memmove_go:                        \n\
            rep     movsb                       \n\
            "
            :
            : "D"(dest), "S"(src), "c"(n)
            : "edx", "memory", "cc"
    );
    return dest;
}

/* int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n)
 * Inputs: const int8_t* s1 = first string to compare
 *         const int8_t* s2 = second string to compare
 *               uint32_t n = number of bytes to compare
 * Return Value: A zero value indicates that the characters compared
 *               in both strings form the same string.
 *               A value greater than zero indicates that the first
 *               character that does not match has a greater value
 *               in str1 than in str2; And a value less than zero
 *               indicates the opposite.
 * Function: compares string 1 and string 2 for equality */
int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n) {
    int32_t i;
    for (i = 0; i < n; i++) {
        if ((s1[i] != s2[i]) || (s1[i] == '\0') /* || s2[i] == '\0' */) {

            /* The s2[i] == '\0' is unnecessary because of the short-circuit
             * semantics of 'if' expressions in C.  If the first expression
             * (s1[i] != s2[i]) evaluates to false, that is, if s1[i] ==
             * s2[i], then we only need to test either s1[i] or s2[i] for
             * '\0', since we know they are equal. */
            return s1[i] - s2[i];
        }
    }
    return 0;
}

/* int8_t* strcpy(int8_t* dest, const int8_t* src)
 * Inputs:      int8_t* dest = destination string of copy
 *         const int8_t* src = source string of copy
 * Return Value: pointer to dest
 * Function: copy the source string into the destination string */
int8_t* strcpy(int8_t* dest, const int8_t* src) {
    int32_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

/* int8_t* strcpy(int8_t* dest, const int8_t* src, uint32_t n)
 * Inputs:      int8_t* dest = destination string of copy
 *         const int8_t* src = source string of copy
 *                uint32_t n = number of bytes to copy
 * Return Value: pointer to dest
 * Function: copy n bytes of the source string into the destination string */
int8_t* strncpy(int8_t* dest, const int8_t* src, uint32_t n) {
    int32_t i = 0;
    while (src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

// /* void test_interrupts(void)
//  * Inputs: void
//  * Return Value: void
//  * Function: increments video memory. To be used to test rtc */
// void test_interrupts(void) {
//     int32_t i;
//     for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
//         video_mem[i << 1]++;
//     }

//     outb(REGISTER_C, RTC_PORT);
//     inb(CMOS_PORT); // reading value from register C to ensure RTC interrupts happen again

//     send_eoi(RTC_IRQ_NUM); //send eoi to correct IRQ line
// }
