#include "io.h"

#include "pins.h"

#include <avr/io.h>

typedef enum {
	PM_IN,
	PM_OUT,
} pin_mode;

static volatile uint8_t *port_addr(mcu_port port);
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
	// set_pin_mode(&pin_ref_div0, PM_OUT);
	set_pin_mode(&pin_ref_div1, PM_OUT);
	set_pin_mode(&pin_ref_div2, PM_OUT);
	set_pin_mode(&pin_ref_trig, PM_IN);

	// Is correct?
	set_pin_mode(&pin_disp_sda, PM_IN);
	set_pin_mode(&pin_disp_scl, PM_IN);
}

logic io_pin_read(const pin *p) {
	uint8_t mask = 1u << p->bit;
	volatile uint8_t *port = port_addr(p->port);

	return (*port & mask) ? L_HIGH : L_LOW;
}

void io_pin_write(const pin *p, logic data) {
	uint8_t mask = 1u << p->bit;
	volatile uint8_t *port = port_addr(p->port);
	
	if (data == L_HIGH) {
		*port |= mask;
	} else {
		*port &= ~mask;
	}
}

static volatile uint8_t *port_addr(mcu_port port) {
	switch (port) {
	case MP_A:
		return &PORTA;
	case MP_B:
		return &PORTB;
	case MP_C:
		return &PORTC;
	}
}

static void set_pin_mode(const pin *p, pin_mode mode) {
	uint8_t mask = 1u << p->bit;
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

	if (mode == PM_OUT) {
		*port |= mask;
	} else {
		*port &= ~mask;
	}
}
