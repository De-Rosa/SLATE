#ifndef _KERNEL_PS2_H
#define _KERNEL_PS2_H

#include <stdint.h>

void initialise_controller(void);
uint8_t read_response(void);

#endif
