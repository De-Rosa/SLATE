#ifndef KERNEL_TASKSCHEDULING_TCB_H
#define KERNEL_TASKSCHEDULING_TCB_H
#include <stdint.h>

#define KERNEL_STACK_SIZE 4096

typedef enum {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_SLEEPING
} task_state;

typedef struct thread_control_block {
    uint32_t *esp;
    uint32_t cr3;
    uint32_t esp0;
    uint32_t pid;
    char *name;
    uint8_t state;
    tcb *next;
} tcb;

#endif