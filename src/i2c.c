/**
 * @file i2c.c
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief Software I2C implementation.
 */
#include "i2c.h"

#include <avr/delay.h>
#include <stdint.h>

#include "io.h"
#include "pins.h"
#include "util.h"

#define PROP_DELAY 2 // us

static inline void scl(logic x);
static inline void sda(logic x);
static inline logic sda_r(void);

void i2c_start(void) {
	// START condition
	// SCL SDA -> SCL sda -> scl sda
	sda(L_LOW);
	delay_us(PROP_DELAY);
	scl(L_LOW);
}

void i2c_stop(void) {
	// STOP condition
	// scl SDA -> SCL sda -> SCL SDA
	sda(L_LOW);
	scl(L_HIGH);
	delay_us(2);
	sda(L_HIGH);
}

logic i2c_write(uint8_t data) {
	// Initial state: scl SDA
	for (uint8_t i = 0; i < 8; i++) {
		// I2C is big endian
		sda(bitget(data, 7 - i));

		// Clock the bit
		delay_us(2);
		scl(L_HIGH);
		delay_us(2);
		scl(L_LOW);
	}

	// Release SDA
	sda(L_HIGH);

	// Receive ACK bit from slave
	// ACK = pulled LOW, NACK = stays HIGH
	delay_us(2);
	scl(L_HIGH);
	delay_us(2);
	logic ack = !sda_r();
	scl(L_LOW);

	return ack;
}

uint8_t i2c_read(void) {
	// Initial state: scl SDA
	uint8_t recv = 0;

	for (uint8_t i = 0; i < 8; i++) {
		// Clock the bit
		delay_us(2);
		scl(L_HIGH);
		delay_us(2);

		// I2C is big endian
		uint32_t bit = (uint32_t)sda_r();
		recv |= bit;
		recv <<= 1;

		scl(L_LOW);
	}

	// Send ACK
	sda(L_LOW);
	delay_us(2);
	scl(L_HIGH);
	delay_us(2);
	scl(L_LOW);

	// Release SDA
	sda(L_HIGH);

	return recv;
}

/**
 * @brief SCL write wrapper.
 *
 * @param[in] x - Logic state.
 */
static inline void scl(logic x) {
	io_pin_mode(&pin_disp_scl, x ? PC_INPUT : PC_OUTPUT);
}

/**
 * @brief SDA write wrapper.
 *
 * @param[in] x - Logic state
 */
static inline void sda(logic x) {
	io_pin_mode(&pin_disp_sda, x ? PC_INPUT : PC_OUTPUT);
}

/**
 * @brief SDA read wrapper.
 *
 * @return Logic state.
 */
static inline logic sda_r(void) {
	return io_pin_read(&pin_disp_sda);
}
