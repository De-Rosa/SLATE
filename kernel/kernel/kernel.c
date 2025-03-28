#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	setup_gdt();
	terminal_initialize();
	printf("Welcome to SLATE!\n");
}
