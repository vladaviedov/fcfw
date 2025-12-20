#pragma once

#include <stdint.h>

typedef enum {
	MP_A,
	MP_B,
	MP_C,
} mcu_port;

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
extern const pin pin_ref_div0;
extern const pin pin_ref_div1;
extern const pin pin_ref_div2;
extern const pin pin_ref_trig;

// Display
extern const pin pin_disp_sda;
extern const pin pin_disp_scl;
