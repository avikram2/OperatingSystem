#ifndef PAGING_H
#define PAGING_H

#include "lib.h"

void enable_paging();


void loadPageDirectory(unsigned int* addr);


void enablePaging();

#endif /* PAGING_H */
