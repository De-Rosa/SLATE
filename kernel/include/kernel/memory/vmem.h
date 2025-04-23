#ifndef VMEM_H
#define VMEM_H

void vmem_init();
void vmem_map(void* virt, void* phys);
void vmem_unmap(void* virt, void*phys);

#endif