// https://wiki.osdev.org/Interrupt_Descriptor_Table

#include <stdint.h> 

#include <stdio.h>
#include <kernel/interrupt/idt.h>
#include <kernel/tty.h>
#include <string.h>

#define PIC1	0x20
#define PIC2	0xA0
#define PIC1_DATA PIC1 + 1
#define PIC2_DATA PIC2 + 1

#define PIC_INIT 0x11
#define MODE_8086 0x01
#define PIC_EOI	0x20

uint8_t idt[8 * 256];
extern void setIDT(uint16_t limit, uint32_t base);
extern void* isr_table[];
extern void* irq_table[];
extern void outb(uint16_t port, uint8_t operand);

void encode_idt_entry(uint8_t *target, struct idt_entry source) {
	// Offset, 32 bits
	target[0] = source.offset & 0xFF; 
	target[1] = (source.offset >> 8) & 0xFF;
	target[6] = (source.offset >> 16) & 0xFF;
	target[7] = (source.offset >> 24) & 0xFF;

	// Selector, 16 bits (should always be 0x08)
	target[2] = source.selector & 0xFF;
	target[3] = (source.selector >> 8) & 0xFF;

	// Zero, 8 bits
	target[4] = 0;

	// Attributes (P, DPL, 0, Gate Type)
	target[5] = source.attributes;
}

void setup_pics(void) {
	// initialise command for PICs (0x11)
	outb(PIC1, PIC_INIT);
	outb(PIC2, PIC_INIT);
	
	// PIC vector offsets (must be divisible by 8)
	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);

	// tell master PIC there is a slave PIC at 0x04
	outb(PIC1_DATA, 0x04);

	// tell slave PIC its cascade identity 0x02
	outb(PIC2_DATA, 0x02);

	// use 8086 mode instead of 8080
	outb(PIC1_DATA, MODE_8086);
	outb(PIC2_DATA, MODE_8086);

	// unmask both PICs
	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);
}

void encode_isrs(uint8_t* idt) {
	for (int i = 0; i < 32; i++) {
		struct idt_entry source = {.offset = (uint32_t) isr_table[i], .selector = 0x08, .attributes = 0x8E};
		encode_idt_entry(&idt[i * 8], source);
	}
}

void encode_irqs(uint8_t* idt) {
	for (int i = 0; i < 16; i++) {
		struct idt_entry source = {.offset = (uint32_t) irq_table[i], .selector = 0x08, .attributes = 0x8E};
		encode_idt_entry(&idt[(i + 32) * 8], source);
	}
}

void setup_idt(void) {
	terminal_info("Initialising IDT...\n");

	setup_pics();

	encode_isrs(idt);
	encode_irqs(idt);

	uint16_t limit = (sizeof(idt) - 1);
	uint32_t base = (uint32_t)&idt;

	setIDT(limit, base);
}
