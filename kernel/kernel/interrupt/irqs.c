#include <kernel/interrupt/keyboard/ps2.h>
#include <kernel/interrupt/keyboard/keyboard.h>
#include <kernel/interrupt/irqs.h>
#include <stdint.h>

extern void outb(uint16_t port, uint8_t operand);

void install_irqs(void) {
	install_irq_handler(1, keyboard_irq);
}

