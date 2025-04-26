#include <kernel/memory/pmem.h>
#include <string.h>

#define PAGE_SIZE 4096
#define PMEM_START 0x100000 // This may need to change..

static uint8_t* bitmap = NULL;
static uint32_t total_pages = 0;

void pmem_init(uint32_t memory_size){
    total_pages = memory_size / PAGE_SIZE;
    bitmap = (uint32_t*)PMEM_START;
    memset(bitmap, 0, total_pages / 8);
}

// BITMAP OPERATIONS :D

static void set_bit(uint32_t index){
    bitmap[index / 8] |= (1 << (index % 8));
};

static void clear_bit(uint32_t index){
    bitmap[index / 8] &= ~(1 << (index % 8));
};

static int is_bit_set(uint32_t index){
    return bitmap[index / 8] & (1 << (index % 8));
};

// FUNCTIONS

void* alloc_page(){
    for (uint32_t i = 0; i < total_pages; i++){     // find unused page, set as used
        while (!is_bit_set(i)){
            set_bit(i);
            return (void*)(PMEM_START + (total_pages / 8) + (i * PAGE_SIZE));
        }
    }
    return NULL; //if no page is free, return NULL (out of memory. we'll deal with this later.)
};

void free_page(void* address){
    uintptr_t offset = (uintptr_t)address - (PMEM_START + total_pages / 8);
    uint32_t index = offset / PAGE_SIZE;

    if (index < total_pages) {
        clear_bit(index);
    }
};

// REPORTING

int get_pmem_usage(){
    int used = 0;
    for (uint32_t i = 0; i < total_pages; i++){     // find used pages
        if (is_bit_set(i)){
            used++;
        }
    }
    return used;
}