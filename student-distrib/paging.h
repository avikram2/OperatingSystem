#ifndef PAGING_H
#define PAGING_H

#include "lib.h"

void enable_paging();

void blank_table(uint32_t* table);

void loadPageDirectory(unsigned int* addr);

void enablePaging();

#endif /* PAGING_H */
