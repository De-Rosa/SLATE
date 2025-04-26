#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdint.h>
#include <kernel/interrupt/idt.h>

char* scan_code_to_key(uint8_t byte);
void keyboard_irq(struct registers* regs);

#endif
