// http://www.osdever.net/bkerndev/Docs/irqs.htm

#include <stdio.h>
#include <stdint.h>
#include <kernel/interrupt/idt.h>
#include <kernel/interrupt/irqs.h>

#define PIC1	0x20
#define PIC2	0xA0
#define PIC1_DATA PIC1 + 1
#define PIC2_DATA PIC2 + 1

#define PIC_EOI 0x20

extern void outb(uint16_t port, uint8_t operand);

void* irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void install_irq_handler(int irq, void (*handler)(struct registers* reg)) {
	if (irq > 15) return;
	irq_routines[irq] = handler;
}

void uninstall_irq_handler(int irq) {
	if (irq > 15) return;
	irq_routines[irq] = 0;
}

void irq_handler(struct registers* regs) {
	void (*handler)(struct registers* regs);
	handler = irq_routines[regs->int_no - 32];
	if (handler) handler(regs);

	// if greater than 40, send EOI to PIC2
	if (regs->int_no >= 40) {
		outb(PIC2, PIC_EOI);
	}

	// send EOI to PIC1
	outb(PIC1, PIC_EOI);
}
