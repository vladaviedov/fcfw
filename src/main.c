/**
 * @file main.c
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version dev
 * @date 2025
 * @license GPLv3.0
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "io.h"
#include "meas.h"
#include "pins.h"
#include "util.h"
#include "ssd1306.h"

static volatile uint8_t trig_flag = 0;
static volatile uint8_t hold_btn_flag = 0;
static volatile uint8_t hold_state = 0;

#define BTN_THRESH 100
static uint8_t btn_counter = 0;

int main(void) {
	io_init();
	meas_setup_default();

	io_pin_write(&pin_io_acq_led, L_HIGH);
	io_pin_write(&pin_io_hold_led, L_LOW);

	if (!ssd1306_init()) {
		uint8_t flag = 0;
		while (1) {
			if (flag) {
				io_pin_write(&pin_io_acq_led, L_HIGH);
				io_pin_write(&pin_io_hold_led, L_HIGH);
			} else {
				io_pin_write(&pin_io_acq_led, L_LOW);
				io_pin_write(&pin_io_hold_led, L_LOW);
			}
			flag = !flag;
			delay_ms(100);
		}
	}

	ssd1306_render_result(0);
	while (1) {
		if (hold_btn_flag) {
			if (!io_pin_read(&pin_io_hold_btn)) {
				btn_counter++;
			} else {
				hold_btn_flag = 0;
				btn_counter = 0;
			}

			if (btn_counter >= BTN_THRESH) {
				hold_state = !hold_state;
				hold_btn_flag = 0;
				btn_counter = 0;

				if (hold_state) {
					io_pin_write(&pin_io_acq_led, L_LOW);
					io_pin_write(&pin_io_hold_led, L_HIGH);
				} else {
					io_pin_write(&pin_io_acq_led, L_HIGH);
					io_pin_write(&pin_io_hold_led, L_LOW);
				}
			}
		}

		if (trig_flag) {
			trig_flag = 0;
			if (hold_state) {
				continue;
			}

			meas_capture();
			uint32_t result = meas_load();
			ssd1306_render_result(result);
		}
	}
}

ISR(PCINT0_vect) {
	// Digital encoder interrupts
	if (!io_pin_read(&pin_io_div_a)) {
	}
	if (!io_pin_read(&pin_io_hold_btn)) {
		hold_btn_flag = 1;
	}
}

ISR(PCINT2_vect) {
	// Reference clock trigger
	if (!io_pin_read(&pin_ref_trig)) {
		trig_flag = 1;
	}
}
