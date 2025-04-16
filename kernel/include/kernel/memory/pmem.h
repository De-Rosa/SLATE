void pmem_init(u_int32_t mem_size);
void* alloc_page(u_int32_t needed_size);
void free_page(void* page);