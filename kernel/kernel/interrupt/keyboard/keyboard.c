// https://wiki.osdev.org/PS/2_Keyboard

#include <kernel/interrupt/keyboard/keyboard.h>
#include <kernel/interrupt/keyboard/ps2.h>
#include <kernel/interrupt/idt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

bool released = false;
bool capitalised = false;

// scan code set 2
char* scan_code_to_key(uint8_t byte) {
	switch (byte) {
		case (0x0E):
			return capitalised ? "~" : "`";
		// Left shift
		case (0x12):
			capitalised = !capitalised;
			return "";
		case (0x15):
			return capitalised ? "Q" : "q";
		case (0x16):
			return capitalised ? "!" : "1";
		case (0x1A):
			return capitalised ? "Z" : "z";
		case (0x1B):
			return capitalised ? "S" : "s";
		case (0x1C):
			return capitalised ? "A" : "a";
		case (0x1D):
			return capitalised ? "W" : "w";
		case (0x1E):
			return capitalised ? "@" : "2";
		case (0x21):
			return capitalised ? "C" : "c";
		case (0x22):
			return capitalised ? "X" : "x";
		case (0x23):
			return capitalised ? "D" : "d";
		case (0x24):
			return capitalised ? "E" : "e";
		case (0x25):
			return capitalised ? "$" : "4";
		case (0x26):
			return capitalised ? "#" : "3";
		case (0x29):
			return " ";
		case (0x2A):
			return capitalised ? "V" : "v";
		case (0x2B):
			return capitalised ? "F" : "f";
		case (0x2C):
			return capitalised ? "T" : "t";
		case (0x2D):
			return capitalised ? "R" : "r";
		case (0x2E):
			return capitalised ? "%" : "5";
		case (0x31):
			return capitalised ? "N" : "n";
		case (0x32):
			return capitalised ? "B" : "b";
		case (0x33):
			return capitalised ? "H" : "h";
		case (0x34):
			return capitalised ? "G" : "g";
		case (0x35):
			return capitalised ? "Y" : "y";
		case (0x36):
			return capitalised ? "^" : "6";
		case (0x3A):
			return capitalised ? "M" : "m";
		case (0x3B):
			return capitalised ? "J" : "j";
		case (0x3C):
			return capitalised ? "U" : "u";
		case (0x3D):
			return capitalised ? "&" : "7";
		case (0x3E):
			return capitalised ? "*" : "8";
		case (0x41):
			return capitalised ? "<" : ",";
		case (0x42):
			return capitalised ? "K" : "k";
		case (0x43):
			return capitalised ? "I" : "i";
		case (0x44):
			return capitalised ? "O" : "o";
		case (0x45):
			return capitalised ? ")" : "0";
		case (0x46):
			return capitalised ? "(" : "9";
		case (0x49):
			return capitalised ? ">" : ".";
		case (0x4A):
			return capitalised ? "?" : "/";
		case (0x4B):
			return capitalised ? "L" : "l";
		case (0x4C):
			return capitalised ? ":" : ";";
		case (0x4D):
			return capitalised ? "P" : "p";
		case (0x4E):
			return capitalised ? "_" : "-";
		case (0x52):
			return capitalised ? "\"" : "'";
		case (0x54):
			return capitalised ? "{" : "[";
		// Caps lock
		case (0x58):
			capitalised = !capitalised;
			return "";
		// Right shift
		case (0x59):
			capitalised = !capitalised;
			return "";
		case (0x5A):
			return "\n";
		case (0x5B):
			return capitalised ? "}" : "]";
		case (0x5D):
			return capitalised ? "|" : "\\";
		default:
			return "?";
	}
}

void modifier_release(uint8_t byte) {
	switch (byte) {
		// Left shift
		case (0x12):
			capitalised = !capitalised;
			return;
		// Right shift
		case (0x59):
			capitalised = !capitalised;
			return;
	}
}

void keyboard_irq(struct registers* regs) {
	uint8_t scancode = read_response();

	if (released) {
		modifier_release(scancode);
		released = false;
		return;
	}

	if (scancode == 0xF0) {
		released = true;
		return;
	}
	char* character = scan_code_to_key(scancode);
	printf(character);
}
