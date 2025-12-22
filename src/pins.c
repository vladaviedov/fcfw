#include "pins.h"

const pin pin_io_hold_btn = { .port = MP_A, .bit = 0 };
const pin pin_io_div_a = { .port = MP_A, .bit = 1 };
const pin pin_io_div_b = { .port = MP_A, .bit = 2 };
const pin pin_io_hold_led = { .port = MP_A, .bit = 3 };
const pin pin_io_acq_led = { .port = MP_B, .bit = 3 };

const pin pin_s_data = { .port = MP_A, .bit = 4 };
const pin pin_s_clk = { .port = MP_A, .bit = 5 };
const pin pin_s_load = { .port = MP_A, .bit = 6 };

const pin pin_ctr_reset = { .port = MP_A, .bit = 7 };

const pin pin_ref_reset = { .port = MP_B, .bit = 0 };
// const pin pin_ref_div0 = { .port = MP_C, .bit = 3 };
const pin pin_ref_div1 = { .port = MP_C, .bit = 4 };
const pin pin_ref_div2 = { .port = MP_C, .bit = 5 };
const pin pin_ref_trig = { .port = MP_C, .bit = 0 };

const pin pin_disp_sda = { .port = MP_B, .bit = 1 };
const pin pin_disp_scl = { .port = MP_C, .bit = 1 };
