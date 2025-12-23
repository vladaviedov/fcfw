#pragma once

typedef enum {
	L_LOW = 0,
	L_HIGH = 1,
} logic;

#define delay_us _delay_us
#define delay_ms _delay_ms

// Bit manipulations
#define bitget(x, i) (((x) & (1u << (i))) ? L_HIGH : L_LOW)
#define bitset(x, i, val) ((val) ? ((x) |= (1u << (i))) : ((x) &= ~(1u << (i))))
