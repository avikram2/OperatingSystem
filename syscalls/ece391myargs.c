#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 125
int main ()
{
    uint8_t buf[BUFSIZE];

        ece391_getargs (buf, BUFSIZE);
	ece391_fdputs (1, buf);
    return 0;
}

