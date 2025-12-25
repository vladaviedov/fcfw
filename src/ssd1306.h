/**
 * @file ssd1306.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief SSD1306 driver (I2C).
 */
#pragma once

#include "util.h"

/**
 * @brief Initialize display.
 *
 * @return L_HIGH - No errors.\n
 *         L_LOW - Error during init.
 */
logic ssd1306_init(void);

/**
 * @brief Display numeric data to user.
 *
 * @param[in] data - Counter data.
 */
void ssd1306_render_result(uint32_t data);
