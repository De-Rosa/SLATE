#include <kernel/idt.h>
#include <kernel/interrupt/irqs.h>
#include <stdio.h>

void keyboard_irq(struct registers* regs) {
	printf("KEYBOARD\n");
}

void install_irqs(void) {
	install_irq_handler(1, keyboard_irq);
}

