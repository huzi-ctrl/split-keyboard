/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#ifndef PKB_STATUS_H
#define PKB_STATUS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief PKB hardware configuration constants for Seeed Studio XIAO nRF52840
 *        with PCF8574AP I2C matrix row expander, SK6812MINI RGB underglow,
 *        and SSD1306 128x32 I2C OLED display.
 */
#define PKB_MATRIX_ROWS          5U
#define PKB_MATRIX_COLS          5U
#define PKB_KEYS_PER_HALF        21U
#define PKB_TOTAL_KEYS           42U
#define PKB_RGB_LED_COUNT        21U
#define PKB_PCF8574_I2C_ADDR     0x38U
#define PKB_SSD1306_I2C_ADDR     0x3CU

int pkb_board_status_init(void);
bool pkb_is_initialized(void);

#endif /* PKB_STATUS_H */
