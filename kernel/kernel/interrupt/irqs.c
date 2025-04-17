#include <kernel/interrupt/keyboard/ps2.h>
#include <kernel/interrupt/idt.h>
#include <kernel/interrupt/irqs.h>
#include <stdio.h>
#include <stdint.h>

extern void outb(uint16_t port, uint8_t operand);

void keyboard_irq(struct registers* regs) {
	uint8_t scancode = read_response();
	printf("a");
}

void install_irqs(void) {
	install_irq_handler(1, keyboard_irq);
}

