/**
 * @file pins.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief Frequency counter MCU pinout.
 */
#pragma once

#include <stdint.h>

/**
 * @enum mcu_port
 * Internal MCU I/O group.
 *
 * @var mcu_port::MP_A
 * Port A (PA0-PA7)
 *
 * @var mcu_port::MP_B
 * Port B (PB0-PB3)
 *
 * @var mcu_port::MP_C
 * Port B (PC0-PC5)
 */
typedef enum {
	MP_A,
	MP_B,
	MP_C,
} mcu_port;

/**
 * @struct pin
 * Hardware pin definition.
 *
 * @var pin::port
 * Internal MCU port.
 *
 * @var pin::bit
 * Index inside of the port.
 */
typedef struct {
	mcu_port port;
	uint8_t bit;
} pin;

// IO
extern const pin pin_io_hold_btn;
extern const pin pin_io_div_a;
extern const pin pin_io_div_b;
extern const pin pin_io_hold_led;
extern const pin pin_io_acq_led;

// Shift register
extern const pin pin_s_data;
extern const pin pin_s_clk;
extern const pin pin_s_load;

// Counter
extern const pin pin_ctr_reset;

// Time reference
extern const pin pin_ref_reset;
// extern const pin pin_ref_div0;
extern const pin pin_ref_div1;
extern const pin pin_ref_div2;
extern const pin pin_ref_trig;

// Display
extern const pin pin_disp_sda;
extern const pin pin_disp_scl;
