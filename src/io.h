#pragma once

#include "pins.h"
#include "util.h"

void io_init(void);
logic io_pin_read(const pin *p);
void io_pin_write(const pin *p, logic data);
