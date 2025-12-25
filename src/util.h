/**
 * @file util.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief Shared utilities.
 */
#pragma once

#include <avr/delay.h>

/**
 * @enum logic
 * Digital logic type.
 *
 * @var logic::L_LOW
 * Digital LOW signal.
 *
 * @var logic::L_HIGH
 * Digital HIGH signal.
 */
typedef enum {
	L_LOW = 0,
	L_HIGH = 1,
} logic;

// Remove leading underscore
#define delay_us _delay_us
#define delay_ms _delay_ms

// Bit manipulations
#define bitget(x, i) (((x) & (1u << (i))) ? L_HIGH : L_LOW)
#define bitset(x, i, val) ((val) ? ((x) |= (1u << (i))) : ((x) &= ~(1u << (i))))
