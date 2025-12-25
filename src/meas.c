/**
 * @file meas.c
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief Frequency counter measurement unit driver.
 */
#include "meas.h"

#include <util/delay.h>

#include "io.h"
#include "pins.h"
#include "util.h"

static uint32_t correction = 0;
static uint8_t fresh = 0;

void meas_setup_default(void) {
	// Setup reference to 2 Hz
	// DIV0 is stuck on high
	// io_pin_write(&pin_ref_div0, L_HIGH);
	io_pin_write(&pin_ref_div1, L_HIGH);
	io_pin_write(&pin_ref_div2, L_LOW);

	// Serializer default states
	io_pin_write(&pin_s_clk, L_LOW);
	io_pin_write(&pin_s_load, L_HIGH);

	// Peform resets
	io_pin_write(&pin_ref_reset, L_HIGH);
	io_pin_write(&pin_ctr_reset, L_HIGH);
	delay_us(1);
	io_pin_write(&pin_ref_reset, L_LOW);
	io_pin_write(&pin_ctr_reset, L_LOW);

	meas_invalidate();
}

void meas_setup_gate(gate_speed gs) {
	uint8_t val = (uint8_t)gs;

	// io_pin_write(&pin_ref_div0, bitget(val, 0));
	io_pin_write(&pin_ref_div1, bitget(val, 1));
	io_pin_write(&pin_ref_div2, bitget(val, 2));

	// Reset reference
	io_pin_write(&pin_ref_reset, L_HIGH);
	delay_us(1);
	io_pin_write(&pin_ref_reset, L_LOW);

	meas_invalidate();
}

void meas_invalidate(void) {
	fresh = 0;
}

void meas_capture(void) {
	// Load data from measurement unit
	io_pin_write(&pin_s_load, L_LOW);
	delay_us(1);
	io_pin_write(&pin_s_load, L_HIGH);

	// Reset counter
	io_pin_write(&pin_ctr_reset, L_HIGH);
	delay_us(1);
	io_pin_write(&pin_ctr_reset, L_LOW);
}

uint32_t meas_load(void) {
	uint32_t data = 0;
	uint8_t fresh_old = fresh;

	for (uint8_t i = 0; i < 32; i++) {
		uint32_t bit = (uint32_t)io_pin_read(&pin_s_data);
		data |= bit;
		data <<= 1;

		io_pin_write(&pin_s_clk, L_HIGH);
		io_pin_write(&pin_s_clk, L_LOW);
	}

	// Apply corrections
	// 1. Phase invertion
	if (bitget(data, 1)) {
		data ^= 0b100;
	}

	// 2. Previous read
	uint32_t result = data - correction;

	// Store future correction
	correction = data & 0b11;
	fresh = 1;

	// NOTE: needs to consider gate time setting in future versions
	return fresh_old ? result : 0;
}
