/**
 * @file meas.h
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief Frequency counter measurement unit driver.
 */
#pragma once

#include <stdint.h>

/**
 * @enum gate_speed
 * Measurement unit gate frequency setting.
 */
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

/**
 * @brief Initialize default measurement setup.
 */
void meas_setup_default(void);

/**
 * @brief Change gate speed.
 *
 * @param[in] gs - New gate speed.
 */
void meas_setup_gate(gate_speed gs);

/**
 * @brief Mark corrections as stale. Will require a read cycle before data is
 * available.
 */
void meas_invalidate(void);

/**
 * @brief Sample & hold current counter value.
 */
void meas_capture(void);

/**
 * @brief Retrieve held data from serializer.
 *
 * @return Stored counter value.
 */
uint32_t meas_load(void);
