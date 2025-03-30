// https://wiki.osdev.org/GDT_Tutorial
// https://wiki.osdev.org/Global_Descriptor_Table 
#include <stdint.h> 
#include <stdio.h>

#include <kernel/gdt.h>

struct gdt_entry { 
	// Base states linear address where segment begins.
	uint32_t base; 
	
	// Limit states the maximum addressable unit for this segment.
	uint32_t limit; 
	
	// Access byte (8 bits) in format 76543210
	// 7: PRESENT BIT (P), refers to a valid segment (must be set to 1 for any valid segment)
	// 6 & 5: DESCRIPTOR PRIVILEDGE LEVEL FIELD (DPL), contains the CPU priviledge level of the segment (0 highest to 3 lowest)
	// 4: DESCRIPTOR TYPE BIT (S), descriptor defines a system segment if false otherwise defines a code or data segment
	// 3: EXECUTABLE BIT (E), descriptor defines a data segment if false otherwise defines a code segment which can be executed from
	// 2: DIRECTION/CONFORMING BIT (DC), 
	//	- FOR DATA SELECTORS: represents direction bit, segment grows up if false otherwise grows down (e.g. for growing stacks)
	//	- FOR CODE SELECTORS: represents conforming bit, segment can only be executed from the ring set in DPL if false otherwise can be executed from an equal or lower priviledge level
	// 1: READABLE/WRITABLE BIT (RW),
	//	- FOR DATA SEGMENTS: represents readable bit, read access is not allowed if false otherwise is allowed
	//	- FOR CODE SEGMENTS: represents writable bit, write access is not allowed if false otherwise is allowed
	// 0: ACCESSED BIT (A), CPU will set it when the segment is accessed unless set to 1 in advance
	//
	// For system segments, the format differs slightly:
	// 7: PRESENT BIT (P)
	// 6 & 5: DESCRIPTOR PRIVILEDGE LEVEL FIELD (DPL)
	// 4: DESCRIPTOR TYPE BIT (S)
	// 3 & 2 & 1 & 0: TYPE, type of system segment:
	//	- In 32-bit protected mode:
	//		- 0x1: 16-bit Task State Segment (TSS) (available)
	//		- 0x2: Local Descriptor Table (LDT)
	//		- 0x3: 16-bit Task State Segment (TSS) (busy)
	//		- 0x9: 32-bit Task State Segment (TSS) (available)
	//		- 0xB: 32-bit Task State Segment (TSS) (busy) 
	//	- In Long Mode:
	//		- 0x2: Local Descriptor Table (LDT)
	//		- 0x9: 64-bit Task State Segment (TSS) (available)
	//		- 0xB: 64-bit Task State Segment (TSS) (busy)
	uint8_t access_byte; // 8 bit

	// Flags (4 bits) in format 3210
	// 3: GRANULARITY FLAG (G), indicates that the limit is in 4 KiB blocks (page granularity) if false otherwise in 1 byte blocks (byte granularity)
	// 2: SIZE FLAG (DB), defines a 16-bit protected mode segment if false otherwise defines a 32-bit protected mode segment.
	// 1: LONG-MODE CODE FLAG (L), should always be clear otherwise defines a 64-bit code segment if set. 
	// 0: RESERVED
	uint8_t flags; // 4 bit
}__attribute__((packed));

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
	// Changed flags for kernel & user code segments to use flag of 0xC instead of 0xA, otherwise triple faults.
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

