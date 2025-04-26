#include <kernel/memory/pmem.h>
#include <kernel/memory/vmem.h>

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

static uint32_t *page_directory = 0;

void vmem_init() {
  // clean directory
  page_directory = (uint32_t *)alloc_page();
  for (int i = 0; i < PAGE_ENTRIES; i++) {
    page_directory[i] = 0;
  }

  uintptr_t phys = 0; // identity map first 4 MiB for kernel
  for (uintptr_t virt = 0; virt < 0x400000;
       virt += PAGE_SIZE) { /// might need to change this upper bound
    vmem_map_page(virt, phys, PAGE_PRESENT | PAGE_WRITE);
    phys += PAGE_SIZE;
  }

  // load page directory into CR3
  asm volatile("mov %0, %%cr3" ::"r"(page_directory));

  uint32_t cr0; // enable paging
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000; // set PG bit with bitmask
  asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

void vmem_map_page(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags) {
  uint32_t dir_index = (virt_addr >> 22) & 0x3FF;
  uint32_t table_index = (virt_addr >> 12) & 0x3FF;

  // Check if the page table exists
  uint32_t *page_table;
  if (page_directory[dir_index] & PAGE_PRESENT) {
    // mask off the flags to get physical address of page table
    page_table = (uint32_t *)(page_directory[dir_index] & 0xFFFFF000);
  } else {
    // allocate and clear new page table
    page_table = (uint32_t *)alloc_page();
    for (int i = 0; i < PAGE_ENTRIES; i++) {
      page_table[i] = 0;
    }

    // add page table to page directory
    page_directory[dir_index] = ((uintptr_t)page_table) | flags | PAGE_PRESENT;
  }

  // map the page
  page_table[table_index] =
      (phys_addr & 0xFFFFF000) | (flags & 0xFFF) | PAGE_PRESENT;

  // invalidate TLB for this page
  asm volatile("invlpg (%0)" ::"r"(virt_addr) : "memory");
}

void vmem_unmap_page(uintptr_t virt_addr) {
  uint32_t dir_index = (virt_addr >> 22) & 0x3FF;
  uint32_t table_index = (virt_addr >> 12) & 0x3FF;

  if (!(page_directory[dir_index] & PAGE_PRESENT)) {
    return;
  }

  uint32_t *page_table = (uint32_t *)(page_directory[dir_index] & 0xFFFFF000);
  page_table[table_index] = 0;

  // invalidate TLB
  asm volatile("invlpg (%0)" ::"r"(virt_addr) : "memory");
}

uintptr_t vmem_translate(uintptr_t virt_addr) {
  uint32_t dir_index = (virt_addr >> 22) & 0x3FF;
  uint32_t table_index = (virt_addr >> 12) & 0x3FF;
  uint32_t offset = virt_addr & 0xFFF;

  if (!(page_directory[dir_index] & PAGE_PRESENT)) {
    return 0;
  }

  uint32_t *page_table = (uint32_t *)(page_directory[dir_index] & 0xFFFFF000);

  if (!(page_table[table_index] & PAGE_PRESENT)) {
    return 0;
  }

  uintptr_t phys_base = page_table[table_index] & 0xFFFFF000;
  return phys_base + offset;
}

/// Important functions Functions

void vmem_alloc_page(uintptr_t virt_addr, uint32_t flags) {
  uintptr_t phys_addr = alloc_page();
  vmem_map_page(virt_addr, phys_addr, flags);
}

void vmem_free_page(uintptr_t virt_addr) {
  uintptr_t phys_addr = vmem_translate(virt_addr);
  if (phys_addr) {
    free_page((void *)phys_addr);
    vmem_unmap_page(virt_addr);
  }
}

// linter test
