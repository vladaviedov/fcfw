#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "io.h"

int main(void) {
	io_init();

	while (1) {
		_delay_ms(500);
	}
}
