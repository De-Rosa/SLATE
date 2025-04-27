#include "kernel/taskscheduling/tcb.h"

// create the head and tail of the ready queue which point to NULL (0)
tcb *ready_queue_head = 0;
tcb *ready_queue_tail = 0;

void round_robin(tcb *current_task_TCB) {
    // when there is no current task, there is no need to switch tasks
    if (!current_task_TCB) {
        return;
    }

    // 
    tcb *prev_task = current_task_TCB;
    tcb *next_task = current_task_TCB->next;

    // if there is no next task, loop back to the head of the ready queue
    if (!next_task) {
        next_task = ready_queue_head;
    }

    if (next_task && next_task->state == TASK_READY) {
        switch_tasks(next_task);
    }
}