#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h> 

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

void setup_gdt(void);

#endif
