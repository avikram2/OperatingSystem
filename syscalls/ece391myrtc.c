#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 25
int main ()
{
    int32_t rtc_fd, ret_val, garbage;
    uint8_t buf[BUFSIZE] = "count\n";

    rtc_fd = ece391_open((uint8_t*)"rtc");
    ret_val = 32;
    ret_val = ece391_write(rtc_fd, &ret_val, 4);
    int i = 0;
    while(i < 320)
    {
        ece391_read(rtc_fd, &garbage, 4);
        i++;
		ece391_fdputs (1, buf);
    }
    return 0;
}

