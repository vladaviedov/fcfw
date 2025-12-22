#pragma once

#include <stdint.h>

typedef enum {
	GS_16HZ = 0,
	GS_8HZ = 1,
	GS_4HZ = 2,
	GS_2HZ = 3,
	GS_1S = 4,
	GS_2S = 5,
	GS_4S = 6,
	GS_8S = 7,
} gate_speed;

void meas_setup_default(void);
void meas_setup_gate(gate_speed gs);
void meas_invalidate(void);
void meas_capture(void);
uint32_t meas_load(void);
