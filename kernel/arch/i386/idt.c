// https://wiki.osdev.org/Interrupt_Descriptor_Table

#include <stdint.h> 

#include <kernel/idt.h>

void encode_idt_entry(uint8_t *target, struct idt_entry source) {
	// Offset, 32 bits
	target[0] = source.offset & 0xFF; 
	target[1] = (source.offset >> 8) & 0xFF;
	target[6] = (source.offset >> 16) & 0xFF;
	target[7] = (source.offset >> 24) & 0xFF;

	// Selector, 16 bits
	target[4] = source.selector & 0xFF;
	target[5] = (source.selector >> 8) & 0xFF;

	// Attributes (P, DPL, 0, Gate Type)
	target[2] = source.attributes;
}

uint8_t idt[8 * 256];
extern void setIDT(uint16_t limit, uint32_t base);

void setup_idt(void) {
	uint16_t limit = (sizeof(idt) - 1);
	uint32_t base = (uint32_t)&idt;

	setIDT(limit, base);
}
