/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include "pkb_status.h"

LOG_MODULE_REGISTER(pkb_shield, CONFIG_ZMK_LOG_LEVEL);

static bool pkb_initialized = false;

int pkb_board_status_init(void) {
    pkb_initialized = true;
    LOG_INF("PKB 42-key split keyboard shield initialized (PCF8574 @ 0x%02x, OLED @ 0x%02x, %u LEDs)",
            PKB_PCF8574_I2C_ADDR, PKB_SSD1306_I2C_ADDR, PKB_RGB_LED_COUNT);
    return 0;
}

bool pkb_is_initialized(void) {
    return pkb_initialized;
}

SYS_INIT(pkb_board_status_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
