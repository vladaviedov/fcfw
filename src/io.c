#include "io.h"

#include "pins.h"
#include "util.h"

#include <avr/io.h>

typedef enum {
	PC_INPUT,
	PC_INPUT_PU,
	PC_OUTPUT,
} pin_config;

typedef enum {
	RT_PUE,
	RT_PORT,
	RT_DDR,
	RT_PIN,
} reg_type;

static volatile uint8_t *get_addr(mcu_port port, reg_type reg);
static void set_pin_mode(const pin *p, pin_config mode);

void io_init(void) {
	set_pin_mode(&pin_io_hold_btn, PC_INPUT_PU);
	set_pin_mode(&pin_io_div_a, PC_INPUT_PU);
	set_pin_mode(&pin_io_div_b, PC_INPUT_PU);
	set_pin_mode(&pin_io_hold_led, PC_OUTPUT);
	set_pin_mode(&pin_io_acq_led, PC_OUTPUT);

	set_pin_mode(&pin_s_data, PC_INPUT);
	set_pin_mode(&pin_s_clk, PC_OUTPUT);
	set_pin_mode(&pin_s_load, PC_OUTPUT);

	set_pin_mode(&pin_ctr_reset, PC_OUTPUT);

	set_pin_mode(&pin_ref_reset, PC_OUTPUT);
	// set_pin_mode(&pin_ref_div0, PC_OUTPUT);
	set_pin_mode(&pin_ref_div1, PC_OUTPUT);
	set_pin_mode(&pin_ref_div2, PC_OUTPUT);
	set_pin_mode(&pin_ref_trig, PC_INPUT_PU);

	// Is correct?
	set_pin_mode(&pin_disp_sda, PC_INPUT);
	set_pin_mode(&pin_disp_scl, PC_INPUT);
}

logic io_pin_read(const pin *p) {
	volatile uint8_t *port = get_addr(p->port, RT_PIN);
	return bitget(*port, p->bit);
}

void io_pin_write(const pin *p, logic data) {
	volatile uint8_t *port = get_addr(p->port, RT_PORT);
	bitset(*port, p->bit, data);
}

static volatile uint8_t *get_addr(mcu_port port, reg_type reg) {
	switch (port) {
	case MP_A:
		switch (reg) {
		case RT_PUE:
			return &PUEA;
		case RT_PORT:
			return &PORTA;
		case RT_DDR:
			return &DDRA;
		case RT_PIN:
			return &PINA;
		}
	case MP_B:
		switch (reg) {
		case RT_PUE:
			return &PUEB;
		case RT_PORT:
			return &PORTB;
		case RT_DDR:
			return &DDRB;
		case RT_PIN:
			return &PINB;
		}
	case MP_C:
		switch (reg) {
		case RT_PUE:
			return &PUEC;
		case RT_PORT:
			return &PORTC;
		case RT_DDR:
			return &DDRC;
		case RT_PIN:
			return &PINC;
		}
	}
}

static void set_pin_mode(const pin *p, pin_config mode) {
	volatile uint8_t *ddr = get_addr(p->port, RT_DDR);
	volatile uint8_t *pue = get_addr(p->port, RT_PUE);

	switch (mode) {
	case PC_INPUT_PU:
		bitset(*pue, p->bit, L_HIGH);
		bitset(*ddr, p->bit, L_LOW);
		break;
	case PC_INPUT:
		bitset(*pue, p->bit, L_LOW);
		bitset(*ddr, p->bit, L_LOW);
		break;
	case PC_OUTPUT:
		bitset(*pue, p->bit, L_LOW);
		bitset(*ddr, p->bit, L_HIGH);
		break;
	}
}
