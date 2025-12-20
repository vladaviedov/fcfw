#include "io.h"

#include "pins.h"

#include <avr/io.h>

typedef enum {
	PM_IN,
	PM_OUT,
} pin_mode;

static void set_pin_mode(const pin *p, pin_mode mode);

void io_init(void) {
	set_pin_mode(&pin_io_hold_btn, PM_IN);
	set_pin_mode(&pin_io_div_a, PM_IN);
	set_pin_mode(&pin_io_div_b, PM_IN);
	set_pin_mode(&pin_io_hold_led, PM_OUT);
	set_pin_mode(&pin_io_acq_led, PM_OUT);

	set_pin_mode(&pin_s_data, PM_IN);
	set_pin_mode(&pin_s_clk, PM_OUT);
	set_pin_mode(&pin_s_load, PM_OUT);

	set_pin_mode(&pin_ctr_reset, PM_OUT);

	set_pin_mode(&pin_ref_reset, PM_OUT);
	set_pin_mode(&pin_ref_div0, PM_OUT);
	set_pin_mode(&pin_ref_div1, PM_OUT);
	set_pin_mode(&pin_ref_div2, PM_OUT);
	set_pin_mode(&pin_ref_trig, PM_IN);

	// Is correct?
	set_pin_mode(&pin_disp_sda, PM_IN);
	set_pin_mode(&pin_disp_scl, PM_IN);
}

static void set_pin_mode(const pin *p, pin_mode mode) {
	uint8_t mask = 1 << p->bit;
	volatile uint8_t *port;

	switch (p->port) {
	case MP_A:
		port = &DDRA;
		break;
	case MP_B:
		port = &DDRB;
		break;
	case MP_C:
		port = &DDRC;
		break;
	}

	switch (mode) {
	case PM_IN:
		*port &= ~(mask);
		break;
	case PM_OUT:
		*port |= mask;
		break;
	}
}
