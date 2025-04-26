#include <stdint.h>

// 4 kilobyte stack
__attribute__((aligned(16))) uint8_t kernel_stack[4096];

// grows downwards
uint32_t kernel_stack_ptr = (uint32_t) (kernel_stack + sizeof(kernel_stack));
