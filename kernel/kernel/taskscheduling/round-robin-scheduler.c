#include "kernel/taskscheduling/tcb.h"
#include <stdint.h>

#define TIMER_IRQ 0
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182 // roughly 1.193 MHz
#define DESIRED_FREQUENCY 100 // 100 Hz

void round_robin(tcb *current_task_TCB, tcb *ready_queue_head) {
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


void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}


void timer_interrupt_handler(tcb *current_task_TCB, tcb *ready_queue_head) {
    pic_send_eoi(TIMER_IRQ);

    round_robin(current_task_TCB, ready_queue_head);
}


void pit_init() {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / DESIRED_FREQUENCY);
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}