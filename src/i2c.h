/**
 * @file i2c.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief Software I2C implementation.
 */
#pragma once

#include <stdint.h>

#include "util.h"

// Setup address byte with write flag
#define i2c_addr_wr(x) (((x) << 1) | 0)
// Setup address byte with read flag
#define i2c_addr_rd(x) (((x) << 1) | 1)

/**
 * @brief Send I2C start instruction.
 */
void i2c_start(void);

/**
 * @brief Send I2C stop instruction.
 */
void i2c_stop(void);

/**
 * @brief Send data over I2C.
 *
 * @param[in] data - Byte to send.
 * @return L_HIGH - ACK.\n
 *         L_LOW - NACK.
 */
logic i2c_write(uint8_t data);

/**
 * @brief Read data over I2C.
 *
 * @return Received byte.
 */
uint8_t i2c_read(void);
