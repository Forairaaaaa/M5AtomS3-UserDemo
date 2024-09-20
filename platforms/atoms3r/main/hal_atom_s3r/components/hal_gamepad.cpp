/**
 * @file hal_gamepad.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal_atom_s3r.h"
#include "../hal_config.h"
#include <mooncake.h>
#include <driver/gpio.h>

void HAL_AtomS3R::_gamepad_init()
{
    spdlog::info("gamepad init");

    gpio_reset_pin((gpio_num_t)HAL_PIN_BUTTON_A);
    gpio_set_direction((gpio_num_t)HAL_PIN_BUTTON_A, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)HAL_PIN_BUTTON_A, GPIO_FLOATING);
}

bool HAL_AtomS3R::getButton(GAMEPAD::GamePadButton_t button)
{
    if (button == GAMEPAD::BTN_A)
        return !gpio_get_level((gpio_num_t)HAL_PIN_BUTTON_A);
    return false;
}
