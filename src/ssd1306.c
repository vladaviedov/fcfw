/**
 * @file ssd1306.c
 * @author Vladyslav Aviedov <vladaviedov at protonmail dot com>
 * @version 1.0.0
 * @date 2025
 * @license GPLv3.0
 * @brief SSD1306 driver (I2C).
 */
#include "ssd1306.h"

#include <string.h>

#include "i2c.h"
#include "util.h"

#define I2C_ADDR 0x3c

#define PAGE_SIZE 128

#define CTRL_BYTE 0x00
#define DATA_BYTE 0x40

#define CMD_MUX_RATIO 0xa8
#define CMD_DISP_OFFSET 0xd3
#define CMD_DISP_START_LINE 0x40
#define CMD_INVERT_X 0xa1
#define CMD_INVERT_Y 0xc8
#define CMD_COM_CONFIG 0xda
#define CMD_CONTRAST_SET 0x81
#define CMD_CHG_PUMP_SET 0x8d
#define CMD_DISP_RAM 0xa4
#define CMD_DISP_NORMAL 0xa6
#define CMD_OSC_SET 0xd5
#define CMD_MEM_ADDR_SET 0x20
#define CMD_DISP_OFF 0xae
#define CMD_DISP_ON 0xaf
#define CMD_COL_SET 0x21
#define CMD_PAGE_SET 0x22

#define VAL_CHG_PUMP_ENABLE 0x14
#define VAL_COM_DEFAULT 0x12
#define VAL_MEM_ADDR_PAGE 0x00

static logic send_cmd(uint8_t command);
static logic send_page(const uint8_t *data);
static void render_num(uint8_t *buf, uint8_t num);

logic ssd1306_init(void) {
	if (!send_cmd(CMD_MUX_RATIO) || !send_cmd(0x3f)
		|| !send_cmd(CMD_DISP_OFFSET) || !send_cmd(0x00)
		|| !send_cmd(CMD_DISP_START_LINE) || !send_cmd(CMD_INVERT_X)
		|| !send_cmd(CMD_INVERT_Y) || !send_cmd(CMD_COM_CONFIG)
		|| !send_cmd(VAL_COM_DEFAULT) || !send_cmd(CMD_CONTRAST_SET)
		|| !send_cmd(0x7f) || !send_cmd(CMD_DISP_RAM)
		|| !send_cmd(CMD_DISP_NORMAL) || !send_cmd(CMD_OSC_SET)
		|| !send_cmd(0x80) || !send_cmd(CMD_MEM_ADDR_SET)
		|| !send_cmd(VAL_MEM_ADDR_PAGE) || !send_cmd(CMD_CHG_PUMP_SET)
		|| !send_cmd(VAL_CHG_PUMP_ENABLE) || !send_cmd(CMD_DISP_ON)) {
		return L_LOW;
	}

	// Use entire display for auto-increment
	send_cmd(CMD_COL_SET);
	send_cmd(0x00); // start
	send_cmd(0x7f); // end

	send_cmd(CMD_PAGE_SET);
	send_cmd(0x00); // start
	send_cmd(0x07); // end

	return L_HIGH;
}

void ssd1306_render_result(uint32_t data) {
	uint8_t page[PAGE_SIZE];

	// First page: empty
	memset(page, 0x00, PAGE_SIZE);
	send_page(page);

	// Second page: data
	memset(page, 0x00, PAGE_SIZE);
	uint8_t *trav = page + 128 - 16;
	for (uint32_t i = 0; i < 9; i++) {
		render_num(trav - 4, data % 10);
		trav -= 8;
		data = data / 10;
	}
	send_page(page);

	// Rest also empty
	memset(page, 0x00, PAGE_SIZE);
	for (uint32_t i = 0; i < 6; i++) {
		send_page(page);
	}
}

/**
 * @brief Send command over to the display.
 *
 * @param[in] command - Command.
 * @return L_HIGH - No errors.\n
 *         L_LOW - Errors occured.
 */
static logic send_cmd(uint8_t command) {
	i2c_start();

	if (!i2c_write(i2c_addr_wr(I2C_ADDR)) || !i2c_write(CTRL_BYTE)
		|| !i2c_write(command)) {
		i2c_stop();
		return L_LOW;
	}

	i2c_stop();
	return L_HIGH;
}

/**
 * @brief Send display data over to the display.
 *
 * @param[in] data - Pixel data.
 * @return L_HIGH - No errors.\n
 *         L_LOW - Errors occured.
 */
static logic send_page(const uint8_t *data) {
	// TODO: consider adding logic to check ACK results
	i2c_start();
	i2c_write(i2c_addr_wr(I2C_ADDR));
	i2c_write(DATA_BYTE);
	for (uint32_t i = 0; i < PAGE_SIZE; i++) {
		i2c_write(data[i]);
	}
	i2c_stop();
	return L_HIGH;
}

/**
 * @brief Render a single digit in 4 columns.
 *
 * @param[out] buf - Pixel buffer.
 * @param[in] num - Number to render.
 */
static void render_num(uint8_t *buf, uint8_t num) {
	switch (num) {
	case 0:
		buf[0] = 0xf0 | 0x0f;
		buf[1] = 0x80 | 0x01;
		buf[2] = 0x80 | 0x01;
		buf[3] = 0xf0 | 0x0f;
		break;
	case 1:
		buf[0] = 0x00;
		buf[1] = 0x00;
		buf[2] = 0x00;
		buf[3] = 0xf0 | 0x0f;
		break;
	case 2:
		buf[0] = 0xf0 | 0x01;
		buf[1] = 0x01 | 0x80 | 0x08;
		buf[2] = 0x01 | 0x80 | 0x08;
		buf[3] = 0x0f | 0x80;
		break;
	case 3:
		buf[0] = 0x01 | 0x80 | 0x08;
		buf[1] = 0x01 | 0x80 | 0x08;
		buf[2] = 0x01 | 0x80 | 0x08;
		buf[3] = 0xf0 | 0x0f;
		break;
	case 4:
		buf[0] = 0x0f;
		buf[1] = 0x08;
		buf[2] = 0x08;
		buf[3] = 0xff;
		break;
	case 5:
		buf[0] = 0x0f | 0x80;
		buf[1] = 0x01 | 0x80 | 0x08;
		buf[2] = 0x01 | 0x80 | 0x08;
		buf[3] = 0xf0 | 0x01;
		break;
	case 6:
		buf[0] = 0xf0 | 0x0f;
		buf[1] = 0x01 | 0x80 | 0x08;
		buf[2] = 0x01 | 0x80 | 0x08;
		buf[3] = 0xf0;
		break;
	case 7:
		buf[0] = 0x01;
		buf[1] = 0x01;
		buf[2] = 0x01;
		buf[3] = 0xf0 | 0x0f;
		break;
	case 8:
		buf[0] = 0x0f | 0xf0;
		buf[1] = 0x01 | 0x80 | 0x08;
		buf[2] = 0x01 | 0x80 | 0x08;
		buf[3] = 0x0f | 0xf0;
		break;
	case 9:
		buf[0] = 0x0f;
		buf[1] = 0x01 | 0x80 | 0x08;
		buf[2] = 0x01 | 0x80 | 0x08;
		buf[3] = 0xf0 | 0x0f;
		break;
	}
}
