#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h> 

struct idt_entry { 
	// Represents address of the entry point of the Interrupt Service Routine (ISR).
	uint32_t offset;

	// Points to a valid code segment in the GDT.
	uint16_t selector;

	// 8 bits of reserved space.
	
	// Represents gate type, present bit, cpu priviledge level.
	uint8_t attributes;

}__attribute__((packed));

struct registers {
	// pushed last seperately
	unsigned int gs, fs, es, ds; 

	// pushed by pusha
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 
	
	// pushed using 'push byte'
	unsigned int isr_number, error_code;

	// pushed automatically
	unsigned int eip, cs, eflags, useresp, ss;

}__attribute__((packed));


void setup_idt(void);

#endif
