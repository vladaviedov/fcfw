#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "pins.h"
#include "io.h"

int main(void) {
	io_init();

	while (1) {
		io_pin_write(&pin_io_acq_led, L_HIGH);
		io_pin_write(&pin_io_hold_led, L_HIGH);
		_delay_ms(500);
		io_pin_write(&pin_io_acq_led, L_LOW);
		io_pin_write(&pin_io_hold_led, L_LOW);
		_delay_ms(500);
	}
}
