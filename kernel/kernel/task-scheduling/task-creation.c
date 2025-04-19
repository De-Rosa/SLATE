#include <stdint.h>
#include "kernel/task-scheduling/tcb.h"

uint32_t next_pid = 1;

extern uint32_t current_cr3;
// assuming single address space is being used
// remove this function once different page directories have been introduced
uint32_t clone_page_directory(uint32_t cr3) {
    return cr3;
}

// idle task is running whenever there is no other task running
void idle_task() {
    while (1) {
        asm volatile("hlt");
    }
}

void init_scheduler() {
    // Allocate and initialize the "init" task manually
    tcb *current_task_TCB = (tcb *)kmalloc(sizeof(tcb));

    uint8_t *stack = (uint8_t *)kmalloc(KERNEL_STACK_SIZE);
    uint32_t stack_top = (uint32_t)(stack + KERNEL_STACK_SIZE);

    current_task_TCB->esp = (uint32_t *)stack_top;
    current_task_TCB->cr3 = current_cr3;
    current_task_TCB->esp0 = stack_top;
    current_task_TCB->pid = 0;
    current_task_TCB->name = "idle";
    current_task_TCB->state = TASK_RUNNING;
}

tcb *create_task(void (*entry_point)(void)) {
    tcb *new_tcb = (tcb *)kmalloc(sizeof(tcb));

    uint8_t *stack = (uint8_t *)kmalloc(KERNEL_STACK_SIZE);
    uint32_t stack_top = (uint32_t)(stack + KERNEL_STACK_SIZE);

    uint32_t *esp = (uint32_t *)stack_top;

    *(--esp) = (uint32_t)entry_point;  // EIP
    *(--esp) = 0;  // EBP
    *(--esp) = 0;  // EDI
    *(--esp) = 0;  // ESI
    *(--esp) = 0;  // EBX

    // if we want to also push EAX, ECX, or EDX, I can add them later

    new_tcb->esp = esp;

    new_tcb->cr3 = (uint32_t)clone_page_directory(current_cr3);  // or same CR3 if shared space
    new_tcb->esp0 = stack_top;

    new_tcb->pid = next_pid++;
    new_tcb->name = "new_thread";
    new_tcb->state = TASK_READY;

    return new_tcb;
}