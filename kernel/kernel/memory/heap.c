#include <kernel/memory/heap.h>
#include <stdint.h>

typedef struct BlockHeader {
    size_t size;
    struct BlockHeader* next_block;
    uint8_t is_free;
} BlockHeader;

#define HEAP_SIZE 4096
static uint8_t memory_pool[HEAP_SIZE];

static void* heap_start = memory_pool;
static void* heap_brk = memory_pool;
static void* heap_end = memory_pool + HEAP_SIZE;

static BlockHeader* free_list_head = NULL;

void init_heap(void) {
    heap_start = memory_pool;
    heap_brk = memory_pool;
    heap_end = memory_pool + HEAP_SIZE;
    free_list_head = NULL;
}

static void insert_free_block(BlockHeader* block) {
    block->is_free = 1;
    block->next_block = free_list_head;
    free_list_head = block;
}

static BlockHeader* request_space(size_t size) {
    size_t total_size = sizeof(BlockHeader) + size;
    if ((uint8_t*)heap_brk + total_size > (uint8_t*)heap_end)
        return NULL;

    BlockHeader* block = (BlockHeader*)heap_brk;
    block->size = size;
    block->is_free = 0;
    block->next_block = NULL;

    heap_brk = (uint8_t*)heap_brk + total_size;
    return block;
}

static BlockHeader* find_free_block(size_t size) {
    BlockHeader* current = free_list_head;
    BlockHeader* prev = NULL;

    while (current) {
        if (current->is_free && current->size >= size) {
            if (prev)
                prev->next_block = current->next_block;
            else
                free_list_head = current->next_block;

            current->is_free = 0;
            current->next_block = NULL;
            return current;
        }
        prev = current;
        current = current->next_block;
    }

    return NULL;
}

static void split_block(BlockHeader* block, size_t requested_size) {
    size_t remaining_size = block->size - requested_size - sizeof(BlockHeader);
    if (remaining_size < sizeof(BlockHeader) + 1) return; // too small to split

    BlockHeader* new_block = (BlockHeader*)((uint8_t*)(block + 1) + requested_size);
    new_block->size = remaining_size;
    new_block->is_free = 1;
    new_block->next_block = free_list_head;

    block->size = requested_size;
    free_list_head = new_block;
}

void* kmalloc(size_t size) {
    if (size == 0) return NULL;

    BlockHeader* block = find_free_block(size);
    if (!block) {
        block = request_space(size);
        if (!block) return NULL;
    } else {
        split_block(block, size);
    }

    return (void*)(block + 1);
}

void kfree(void* ptr) {
    if (!ptr) return;
    BlockHeader* block = ((BlockHeader*)ptr) - 1;
    insert_free_block(block);
}
