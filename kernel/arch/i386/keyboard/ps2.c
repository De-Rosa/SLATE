// https://wiki.osdev.org/%228042%22_PS/2_Controller

#include <stdint.h> 
#include <stdio.h> 

#include <kernel/ps2.h>
#include <kernel/tty.h>

// Too large of a wait time causes a general protection fault
#define WAIT_TIME	10000

// used to read data recieved from PS/2 device
#define PS2_DATA_PORT	0x60

// status register (read):
// contains flags which show the state of the controller
// command register (write):
// used to send commands to the PS/2 controller
#define PS2_REGISTER	0x64

#define READ_CONTROLLER_CONFIG		0x20
#define WRITE_CONTROLLER_CONFIG		0x60
#define DISABLE_SECOND_PORT		0xA7
#define ENABLE_SECOND_PORT		0xA8
#define TEST_SECOND_PORT		0xA9
#define TEST_CONTROLLER			0xAA
#define TEST_FIRST_PORT			0xAB
#define DISABLE_FIRST_PORT		0xAD
#define	ENABLE_FIRST_PORT		0xAE
#define	SEND_TO_SECOND_PORT		0xAE
#define RESET_DEVICE			0xFF

#define CONTROLLER_TEST_SUCCESS		0x55
#define PORT_TEST_SUCCESS		0x00


extern void outb(uint16_t port, uint8_t operand);
extern uint8_t inb(uint16_t port);

void wait(int count) {
    for (int i = 0; i < count; i++) {
        asm volatile("nop");
    }
}

// returns 1 if clear, 0 if timed out
int wait_input_buffer(void) {
	for (int i = 0; i < WAIT_TIME; i++) {
		uint8_t status = inb(PS2_REGISTER);
		if ((status & 0b10) == 0) return 1; // input buffer status is clear
	}
	printf("(wait_input_buffer) timed out...\n");
	return 0; // timed out!
}

// returns 1 if clear, 0 if timed out
int wait_output_buffer(void) {
	for (int i = 0; i < WAIT_TIME; i++) {
		uint8_t status = inb(PS2_REGISTER);
		if (status & 1) return 1; // output buffer status is set
	}
	printf("(wait_output_buffer) timed out...\n");
	return 0; // timed out!
}

// returns 0xFF on failure
uint8_t read_response(void) {
	if (!wait_output_buffer()) return 0xFF;
	uint8_t result = inb(PS2_DATA_PORT);
	return result;
}

void write_to_controller(uint8_t command) {
	if (!wait_input_buffer()) return;
	outb(PS2_REGISTER, command & 0xFF);
}

void write_to_controller_double(uint8_t first, uint8_t next) {
	if (!wait_input_buffer()) return;
	outb(PS2_REGISTER, first & 0xFF);

	if (!wait_input_buffer()) return;
	outb(PS2_DATA_PORT, next & 0xFF);
}

// returns 1 if successful, 0 if not
int send_to_first_port(uint8_t data) {
	if (!wait_input_buffer()) return 0;
	outb(PS2_DATA_PORT, data);
	return 1;
}

// returns 1 if successful, 0 if not
int send_to_second_port(uint8_t data) {
	write_to_controller(SEND_TO_SECOND_PORT);

	if (!wait_input_buffer()) return 0;
	outb(PS2_DATA_PORT, data);
	return 1;
}

void initialise_controller(void) {
	terminal_info("Initialising PS/2 controller and ports...\n");

	write_to_controller(DISABLE_FIRST_PORT); // disable first ps/2 port
	write_to_controller(DISABLE_SECOND_PORT); // disable second ps/2 port
	
	// set controller configuration byte
	write_to_controller(READ_CONTROLLER_CONFIG);
	uint8_t controller_config = read_response();
	controller_config &= 0b10101110; // clear bits 0, 4, and 6
	write_to_controller_double(WRITE_CONTROLLER_CONFIG, controller_config);

	// perform test 
	write_to_controller(TEST_CONTROLLER);
	uint8_t response = read_response();
	if (response != CONTROLLER_TEST_SUCCESS) {
		printf("PS/2 Controller Self Test Fail!\n");
		return;
	}

	// check if there are 2 channels
	write_to_controller(ENABLE_SECOND_PORT);
	write_to_controller(READ_CONTROLLER_CONFIG);
	controller_config = read_response();
	int is_dual_channel = 0;

	// is a dual channel device
	if ((controller_config & 0b00100000) != 0b00100000) {
		printf("PS/2 is dual channel.\n");
		is_dual_channel = 1;

		write_to_controller(DISABLE_SECOND_PORT);
		controller_config &= 0b11011101; // clear bits 1 and 5
		write_to_controller_double(WRITE_CONTROLLER_CONFIG, controller_config);
	} else {
		printf("PS/2 is not dual channel.\n");
	}
	
	// test ports 
	write_to_controller(TEST_FIRST_PORT);
	uint8_t test_result = read_response();
	if (test_result != PORT_TEST_SUCCESS) {
		printf("PS/2 First Port Test Fail!");
		if (!is_dual_channel) return;
	} else {
		printf("PS/2 First Port Test Success!\n");
	}

	if (is_dual_channel) {
		write_to_controller(TEST_SECOND_PORT);
		test_result = read_response();
		if (test_result != PORT_TEST_SUCCESS) {
			printf("PS/2 Second Port Test Fail!");
		} else {
			printf("PS/2 Second Port Test Success!\n");
		}
	}

	// enable ports
	write_to_controller(ENABLE_FIRST_PORT);
	if (is_dual_channel) write_to_controller(ENABLE_SECOND_PORT);

	// reset devices
	send_to_first_port(RESET_DEVICE);
	response = read_response();
	if (response != 0xFA && response != 0xAA) {
		switch (response) {
			case 0xFF:
				printf("Timeout when resetting first port PS/2 device! First port not populated.\n");
				return;
			case 0xFC:
				printf("PS/2 first port device self test failed.\n");
				break;
		}
		response = read_response();
	}

	if (is_dual_channel) {
		send_to_second_port(RESET_DEVICE);
		response = read_response();
		if (response != 0xFA && response != 0xAA) {
			switch (response) {
				case 0xFF:
					printf("Timeout when resetting second port PS/2 device! Second port not populated.\n");
					return;
				case 0xFC:
					printf("PS/2 second port device self test failed.\n");
					break;
			}
		}
		response = read_response();
	}

	// enable IRQs
	/*write_to_controller(READ_CONTROLLER_CONFIG);
	controller_config = read_response();
	controller_config |= 1; // set bit 0
	controller_config |= is_dual_channel << 1;
	write_to_controller_double(WRITE_CONTROLLER_CONFIG, controller_config);*/
}
