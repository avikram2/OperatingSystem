#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

int main ()
{
    int x = 0;
    int y = 1;
    y = y / x;
    ece391_fdputs (1, (uint8_t*)"Hello2, if this ran, the program was correct. Yay!\n");

    return 0;
}

