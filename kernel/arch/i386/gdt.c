// https://wiki.osdev.org/GDT_Tutorial
// https://wiki.osdev.org/Global_Descriptor_Table 
#include <stdint.h> 

#include <kernel/gdt.h>

// https://wiki.osdev.org/GDT_Tutorial#Filling_the_Table
// *target refers to the logical address of the Segment Descriptor.
void encode_gdt_entry(uint8_t *target, struct gdt_entry source) {
	if (source.limit > 0xFFFFF) {
		// error !
		return;
	}

    // Encoding the limit.
    // Limit should be the first 16 bits of the entry, then the next 4 after the access byte.
    target[0] = source.limit & 0xFF; // first 8 bits of the limit
    target[1] = (source.limit >> 8) & 0xFF; // next 8 bits of the limit
    target[6] = (source.limit >> 16) & 0x0F; // last 4 bits

    // Encoding the base.
    // Base should be next 24 bits after limit then last 8 bits of entry.
    target[2] = source.base & 0xFF; // first 8 bits of the base
    target[3] = (source.base >> 8) & 0xFF; // next 8 bits of the base
    target[4] = (source.base >> 16) & 0xFF; // next 8 bits of the base
    target[7] = (source.base >> 24) & 0xFF; // last 8 bits

    // Encoding the access byte.
    // Access byte should be next 8 bits after 24 bits of base.
    target[5] = source.access_byte; // all 8 bits

    // Encoding the flags.
    // Flags should be next 4 bits after 4 bits of limit.
    target[6] |= (source.flags << 4); // set all 4 bits using OR
}

void encode_segments(uint8_t *entries) {
	// Templates for each segment descriptor
	// 32 bit
	struct gdt_entry segment_templates[5] = {
		{ .base = 0, .limit = 0x00000000, .access_byte = 0x00, .flags = 0x0 }, // Null Descriptor
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0x9A, .flags = 0xC }, // Kernel Mode Code Segment
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0x92, .flags = 0xC }, // Kernel Mode Data Segment
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0xFA, .flags = 0xC }, // User Mode Code Segment
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0xF2, .flags = 0xC } // User Mode Data Segment
	};

	for (int i = 0; i < 5; i++) {
		// each item in the array is one byte, each segment has offset of 8 bytes
		encode_gdt_entry(&entries[i * 8], segment_templates[i]);
	}
}

uint8_t gdt[8 * 5]; 
extern void setGDT(uint16_t limit, uint32_t base);
extern void reloadSegments(void);

void setup_gdt(void) {
	encode_segments(gdt);

	uint16_t limit = (sizeof(gdt) - 1);
	uint32_t base = (uint32_t)&gdt;

	setGDT(limit, base);
	reloadSegments();
}

