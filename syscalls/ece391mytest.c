#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 25
int main ()
{
    int32_t fd, cnt;
    uint8_t data[BUFSIZE+1];

    if (-1 == (fd = ece391_open ((uint8_t*)"frame0"))) {
        ece391_fdputs (1, (uint8_t*)"file open failed\n");
        return -1;
    }
    while (1) {
        cnt = ece391_read (fd, data, BUFSIZE);
        if(cnt <= 0)
  	{
		return 0;
	}
	ece391_write (1, data, cnt + 1);
    }
	
    return 0;
}

