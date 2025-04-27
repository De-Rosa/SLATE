#include <stdint.h>
#include "kernel/taskscheduling/tcb.h"

uint64_t next_pid = 1;

extern uint64_t current_cr3;
// assuming single address space is being used
// remove this function once different page directories have been introduced
uint64_t clone_page_directory(uint64_t cr3) {
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
    uint64_t stack_top = (uint64_t)(stack + KERNEL_STACK_SIZE);

    current_task_TCB->esp = (uint64_t *)stack_top;
    current_task_TCB->cr3 = current_cr3;
    current_task_TCB->esp0 = stack_top;
    current_task_TCB->pid = 0;
    current_task_TCB->name = "idle";
    current_task_TCB->state = TASK_RUNNING;
}

tcb *create_task(void (*entry_point)(void)) {
    tcb *new_tcb = (tcb *)kmalloc(sizeof(tcb));

    uint8_t *stack = (uint8_t *)kmalloc(KERNEL_STACK_SIZE);
    uint64_t stack_top = (uint64_t)(stack + KERNEL_STACK_SIZE);

    uint64_t *esp = (uint64_t *)stack_top;

    *(--esp) = (uint64_t)entry_point;  // Fake return address (RIP)
    *(--esp) = 0;  // RBP
    *(--esp) = 0;  // RDI
    *(--esp) = 0;  // RSI
    *(--esp) = 0;  // RBX
    *(--esp) = 0;  // R12
    *(--esp) = 0;  // R13
    *(--esp) = 0;  // R14
    *(--esp) = 0;  // R15

    // if we want to also push EAX, ECX, or EDX, I can add them later

    new_tcb->esp = esp;

    new_tcb->cr3 = (uint64_t)clone_page_directory(current_cr3);
    new_tcb->esp0 = stack_top;

    new_tcb->pid = next_pid++;
    new_tcb->name = "new_thread";
    new_tcb->state = TASK_READY;

    return new_tcb;
}
