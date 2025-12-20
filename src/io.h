#pragma once

#include "pins.h"

typedef enum {
	L_HIGH,
	L_LOW,
} logic;

void io_init(void);
logic io_pin_read(const pin *p);
void io_pin_write(const pin *p, logic data);
