/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../view.h"
#include <cstdint>
#include <hal/hal.h>
#include <assets/assets.h>
#include <shared/shared.h>

using namespace VIEW;

void func_uart_t::start()
{
    HAL::StartUartPassThrough(uart_baud_list[uart_mon_mode_index], uart_io_list[uart_mon_mode_index][0],
                              uart_io_list[uart_mon_mode_index][1]);

    HAL::GetDisplay()->drawPng(uart_mon_img_list[uart_mon_mode_index], ~0u, 0, 0);
    HAL::GetDisplay()->setFont(&fonts::Font0);
}

void func_uart_t::update(bool btn_click)
{
    if (btn_click) {
        uart_mon_mode_index++;
        if (uart_mon_mode_index > 3) {
            uart_mon_mode_index = 0;
        }

        HAL::StartUartPassThrough(uart_baud_list[uart_mon_mode_index], uart_io_list[uart_mon_mode_index][0],
                                  uart_io_list[uart_mon_mode_index][1]);

        HAL::GetDisplay()->drawPng(uart_mon_img_list[uart_mon_mode_index], ~0u, 0, 0);
    }

    HAL::UpdateUartPassThrough(
        [](uint8_t c) {
            HAL::GetDisplay()->fillRect(86, 31, 128 - 86, 9);
            HAL::GetDisplay()->setCursor(93, 33);
            HAL::GetDisplay()->setTextColor(
                (HAL::RandomInt(0, 255) << 16 | HAL::RandomInt(0, 255) << 8 | HAL::RandomInt(0, 255)), TFT_BLACK);
            HAL::GetDisplay()->printf("0x%02X", c);
        },
        [](uint8_t c) {
            HAL::GetDisplay()->fillRect(86, 41, 128 - 86, 9, TFT_BLACK);
            HAL::GetDisplay()->setCursor(93, 43);
            HAL::GetDisplay()->setTextColor(
                (HAL::RandomInt(0, 255) << 16 | HAL::RandomInt(0, 255) << 8 | HAL::RandomInt(0, 255)), TFT_BLACK);
            HAL::GetDisplay()->printf("0x%02X", c);
        });

    HAL::Delay(10);
}

void func_uart_t::stop()
{
    HAL::StopUartPassThrough();

    HAL::GetDisplay()->setTextColor(TFT_WHITE, TFT_BLACK);
}
