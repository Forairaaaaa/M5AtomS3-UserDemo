/**
 * @file i2c.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../view.h"
#include <cstdint>
#include <hal/hal.h>
#include <assets/assets.h>
#include <shared/shared.h>

using namespace VIEW;

void func_uart_t::start()
{
    // TO HAL
    // Serial.begin(uart_baud_list[uart_mon_mode_index],
    //              SERIAL_8N1,
    //              uart_io_list[uart_mon_mode_index][0],
    //              uart_io_list[uart_mon_mode_index][1]);
    HAL::StartUartPassThrough(
        uart_baud_list[uart_mon_mode_index], uart_io_list[uart_mon_mode_index][0], uart_io_list[uart_mon_mode_index][1]);

    HAL::GetDisplay()->drawPng(uart_mon_img_list[uart_mon_mode_index], ~0u, 0, 0);
    HAL::GetDisplay()->setFont(&fonts::Font0);
}

void func_uart_t::update(bool btn_click)
{
    if (btn_click)
    {
        uart_mon_mode_index++;
        if (uart_mon_mode_index > 3)
        {
            uart_mon_mode_index = 0;
        }

        // TO HAL
        // Serial.end();
        // Serial.begin(uart_baud_list[uart_mon_mode_index],
        //              SERIAL_8N1,
        //              uart_io_list[uart_mon_mode_index][0],
        //              uart_io_list[uart_mon_mode_index][1]);
        HAL::StartUartPassThrough(
            uart_baud_list[uart_mon_mode_index], uart_io_list[uart_mon_mode_index][0], uart_io_list[uart_mon_mode_index][1]);

        HAL::GetDisplay()->drawPng(uart_mon_img_list[uart_mon_mode_index], ~0u, 0, 0);
    }

    // Grove => USB
    // TO HAL
    // size_t rx_len = Serial.available();
    // if (rx_len)
    // {
    //     for (size_t i = 0; i < rx_len; i++)
    //     {
    //         uint8_t c = Serial.read();
    //         USBSerial.write(c);
    //         HAL::GetDisplay()->fillRect(86, 31, 128 - 86, 9);
    //         HAL::GetDisplay()->setCursor(93, 33);
    //         HAL::GetDisplay()->setTextColor((random(0, 255) << 16 | random(0, 255) << 8 | random(0, 255)), TFT_BLACK);
    //         HAL::GetDisplay()->printf("0x%02X", c);
    //     }
    // }

    // USB => Grove
    // TO HAL
    // size_t tx_len = USBSerial.available();
    // if (tx_len)
    // {
    //     for (size_t i = 0; i < tx_len; i++)
    //     {
    //         uint8_t c = USBSerial.read();
    //         Serial.write(c);
    //         HAL::GetDisplay()->fillRect(86, 41, 128 - 86, 9, TFT_BLACK);
    //         HAL::GetDisplay()->setCursor(93, 43);
    //         HAL::GetDisplay()->setTextColor((random(0, 255) << 16 | random(0, 255) << 8 | random(0, 255)), TFT_BLACK);
    //         HAL::GetDisplay()->printf("0x%02X", c);
    //     }
    // }

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
    // TO HAL
    // Serial.end();
    // gpio_reset_pin((gpio_num_t)1);
    // gpio_reset_pin((gpio_num_t)2);
    HAL::StopUartPassThrough();

    HAL::GetDisplay()->setTextColor(TFT_WHITE, TFT_BLACK);
}
