#ifndef PMEM_H
#define PMEM_H

#include <stdint.h>
#include <stddef.h>

void pmem_init(uint32_t memory_size);
void* alloc_page();
void free_page(void* address);

#endif