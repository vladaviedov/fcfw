/**
 * @file io.c
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version dev
 * @date 2025
 * @license GPLv3.0
 * @brief External I/O.
 */
#include "io.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "pins.h"
#include "util.h"

/**
 * @enum reg_type
 * Register type.
 *
 * @var reg_type::RT_PUE
 * Pull-up resistor register.
 *
 * @var reg_type::RT_PORT
 * Data (output) register.
 *
 * @var reg_type::RT_DDR
 * Data direction register.
 *
 * @var reg_type::RT_PIN
 * Input register.
 */
typedef enum {
	RT_PUE,
	RT_PORT,
	RT_DDR,
	RT_PIN,
	RT_PCMSK,
} reg_type;

static void init_pin_mode(void);
static void init_interrupts(void);
static volatile uint8_t *get_addr(mcu_port port, reg_type reg);
static void enable_interrupt(const pin *p);

void io_init() {
	init_pin_mode();
	init_interrupts();
}

void io_pin_mode(const pin *p, pin_config mode) {
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

logic io_pin_read(const pin *p) {
	volatile uint8_t *port = get_addr(p->port, RT_PIN);
	return bitget(*port, p->bit);
}

void io_pin_write(const pin *p, logic data) {
	volatile uint8_t *port = get_addr(p->port, RT_PORT);
	bitset(*port, p->bit, data);
}

/**
 * @brief Initialize pin directions & pull-ups.
 */
static void init_pin_mode(void) {
	io_pin_mode(&pin_io_hold_btn, PC_INPUT_PU);
	io_pin_mode(&pin_io_div_a, PC_INPUT_PU);
	io_pin_mode(&pin_io_div_b, PC_INPUT_PU);
	io_pin_mode(&pin_io_hold_led, PC_OUTPUT);
	io_pin_mode(&pin_io_acq_led, PC_OUTPUT);

	io_pin_mode(&pin_s_data, PC_INPUT);
	io_pin_mode(&pin_s_clk, PC_OUTPUT);
	io_pin_mode(&pin_s_load, PC_OUTPUT);

	io_pin_mode(&pin_ctr_reset, PC_OUTPUT);

	io_pin_mode(&pin_ref_reset, PC_OUTPUT);
	// io_pin_mode(&pin_ref_div0, PC_OUTPUT);
	io_pin_mode(&pin_ref_div1, PC_OUTPUT);
	io_pin_mode(&pin_ref_div2, PC_OUTPUT);
	io_pin_mode(&pin_ref_trig, PC_INPUT_PU);

	// I2C lines are released, external pull-up
	io_pin_mode(&pin_disp_sda, PC_INPUT);
	io_pin_mode(&pin_disp_scl, PC_INPUT);
	// Ensure that I2C ports are programmed to output LOW when set as output
	io_pin_write(&pin_disp_sda, L_LOW);
	io_pin_write(&pin_disp_scl, L_LOW);
}

/**
 * @brief Initialize I/O interrupts.
 */
static void init_interrupts(void) {
	// Enable Port A & C interrupts
	bitset(GIMSK, PCIE0, L_HIGH);
	bitset(GIMSK, PCIE2, L_HIGH);

	enable_interrupt(&pin_io_hold_btn);
	enable_interrupt(&pin_io_div_a);
	enable_interrupt(&pin_ref_trig);

	sei();
}

/**
 * @brief Get memory address of I/O register.
 *
 * @param[in] port - Port selection.
 * @param[in] reg - Register selection.
 * @return Address of the port + register type combination.
 */
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
		case RT_PCMSK:
			return &PCMSK0;
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
		case RT_PCMSK:
			return &PCMSK1;
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
		case RT_PCMSK:
			return &PCMSK2;
		}
	}
}

/**
 * @brief Enable interrupt on an pin.
 *
 * @param[in] p - I/O pin.
 */
static void enable_interrupt(const pin *p) {
	volatile uint8_t *mask = get_addr(p->port, RT_PCMSK);
	bitset(*mask, p->bit, L_HIGH);
}
