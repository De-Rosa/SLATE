#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
	terminal_initialize();

	setup_gdt();

	printf("Welcome to SLATE!\n");
}
