#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

#include <kernel/memory/pmem.h>
#include <kernel/memory/vmem.h>
#include <multiboot.h>

#include <kernel/interrupt/idt.h>
#include <kernel/interrupt/irqs.h>
#include <kernel/interrupt/keyboard/ps2.h>

void kernel_main(void) {
	terminal_initialize();

	setup_gdt();
	setup_idt();

	install_irqs();
	initialise_controller();

	printf("\nWelcome to SLATE! Keyboard test.\n");

	// Keep running, interrupts will override this.
	for(;;) {
		asm("hlt");
	}
}
