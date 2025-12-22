/**
 * @file io.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version dev
 * @date 2025
 * @license GPLv3.0
 * @brief External I/O.
 */
#pragma once

#include "pins.h"
#include "util.h"

/**
 * @brief Initialize I/O.
 */
void io_init(void);

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
