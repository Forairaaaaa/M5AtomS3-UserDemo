/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

/**
 * @brief Pin configs
 *
 */

// Display
#define HAL_PIN_LCD_MOSI 21
#define HAL_PIN_LCD_MISO -1
#define HAL_PIN_LCD_SCLK 17
#define HAL_PIN_LCD_DC   33
#define HAL_PIN_LCD_CS   15
#define HAL_PIN_LCD_RST  34
#define HAL_PIN_LCD_BUSY -1
#define HAL_PIN_LCD_BL   16

// I2C
#define HAL_PIN_I2C_INTER_SDA 38
#define HAL_PIN_I2C_INTER_SCL 39
#define HAL_PIN_I2C_EXTER_SDA 2
#define HAL_PIN_I2C_EXTER_SCL 1

// Button
#define HAL_PIN_BUTTON_A 41

// IMU
// Hardware pulled up
#define HAL_PIN_IMU_INT 16

// IR
#define HAL_PIN_IR_TX 4
