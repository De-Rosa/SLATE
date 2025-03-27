// https://wiki.osdev.org/GDT_Tutorial
// https://wiki.osdev.org/Global_Descriptor_Table 
#include <stdint.h> 
#include <sys/types.h>

// Each entry in the GDT is 8 bytes long.
struct gdt_entry { 
	// Base states linear address where segment begins.
	unsigned char base; // 32 bit
	
	// Limit states the maximum addressable unit for this segment.
	unsigned int limit; // 20 bit
	
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
	unsigned char access_byte; // 8 bit

	// Flags (4 bits) in format 3210
	// 3: GRANULARITY FLAG (G), indicates that the limit is in 4 KiB blocks (page granularity) if false otherwise in 1 byte blocks (byte granularity)
	// 2: SIZE FLAG (DB), defines a 16-bit protected mode segment if false otherwise defines a 32-bit protected mode segment.
	// 1: LONG-MODE CODE FLAG (L), should always be clear otherwise defines a 64-bit code segment if set. 
	// 0: RESERVED
	unsigned char flags; // 4 bit
};

// https://wiki.osdev.org/GDT_Tutorial#Filling_the_Table
// *target refers to the logical address of the Segment Descriptor.
void encode_gdt_entry(uint8_t *target, struct gdt_entry source) {
	if (source.limit > 0xFFFFF) {
		// error !
		return;
	}

    // Encoding the limit.
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;

    // Encoding the base.
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // Encoding the access byte.
    target[5] = source.access_byte;

    // Encoding the flags.
    target[6] |= (source.flags << 4); // bitwise OR
}

int main(void) {
	// Long mode setup 
	
	// Templates for each segment descriptor
	struct gdt_entry segment_templates[5] = {
		{ .base = 0, .limit = 0x00000000, .access_byte = 0x00, .flags = 0x0 }, // Null Descriptor
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0x9A, .flags = 0xA }, // Kernel Mode Code Segment
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0x92, .flags = 0xC }, // Kernel Mode Data Segment
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0xFA, .flags = 0xA }, // User Mode Code Segment
		{ .base = 0, .limit = 0xFFFFF, .access_byte = 0xF2, .flags = 0xC } // User Mode Data Segment
	};

	// The offsets for each segment entry in the GDT.
	u_int8_t segment_entries[5] = {
		0x0000, // Null Descriptor
		0x0008,	// Kernel Mode Code Segment
		0x0010, // Kernel Mode Data Segment
		0x0018, // User Mode Code Segment
		0x0020 // User Mode Data Segment
	};
	
	for (int i = 0; i < 5; i++) {
		encode_gdt_entry(&segment_entries[i], segment_templates[i]);
	}
}
