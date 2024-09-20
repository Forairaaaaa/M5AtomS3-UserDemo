/**
 * @file hal_config.h
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-06-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

/**
 * @brief Pin configs
 *
 */

// Display
#define HAL_PIN_LCD_MOSI 21
#define HAL_PIN_LCD_MISO -1
#define HAL_PIN_LCD_SCLK 15
#define HAL_PIN_LCD_DC 42
#define HAL_PIN_LCD_CS 14
#define HAL_PIN_LCD_RST 48
#define HAL_PIN_LCD_BUSY -1
#define HAL_PIN_LCD_BL -1

// I2C
#define HAL_PIN_I2C_INTER_SDA 45
#define HAL_PIN_I2C_INTER_SCL 0
#define HAL_PIN_I2C_EXTER_SDA 2
#define HAL_PIN_I2C_EXTER_SCL 1

// Button
#define HAL_PIN_BUTTON_A 41

// IMU
// Hardware pulled up
#define HAL_PIN_IMU_INT 16

// IR
#define HAL_PIN_IR_TX 47