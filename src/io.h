/**
 * @file io.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief External I/O.
 */
#pragma once

#include "pins.h"
#include "util.h"

/**
 * @enum pin_config
 * Pin configuration.
 *
 * @var pin_config::PC_INPUT
 * Configure pin as an input.
 *
 * @var pin_config::PC_INPUT_PU
 * Configure pin as an input (with pull-up).
 *
 * @var pin_config::PC_OUTPUT
 * Configure pin as an output.
 */
typedef enum {
	PC_INPUT,
	PC_INPUT_PU,
	PC_OUTPUT,
} pin_config;

/**
 * @brief Initialize I/O.
 */
void io_init(void);

/**
 * @brief Set I/O pin mode.
 *
 * @param[in] p - I/O pin.
 * @param[in] mode - Pin configuration mode.
 */
void io_pin_mode(const pin *p, pin_config mode);

/**
 * @brief Read I/O pin.
 *
 * @param[in] p - I/O pin.
 * @return Pin logic level.
 */
logic io_pin_read(const pin *p);

/**
 * @brief Write I/O pin.
 *
 * @param[in] p - I/O pin.
 * @param[in] data - Logic level to write.
 */
void io_pin_write(const pin *p, logic data);
