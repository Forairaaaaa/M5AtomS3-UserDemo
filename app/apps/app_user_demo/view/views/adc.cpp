/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../view.h"
#include <hal/hal.h>
#include <assets/assets.h>
#include <shared/shared.h>

using namespace VIEW;

void func_adc_t::start()
{
    HAL::GetDisplay()->drawPng(AssetPool::GetImage().AppUserDemo.io_adc_01_img, ~0u, 0, 0);
    HAL::GetDisplay()->setFont(&fonts::efontCN_16_b);
}

void func_adc_t::update(bool btn_click)
{
    if (HAL::Millis() - last_update_time < 90) {
        return;
    }

    ch1_vol = 0;
    ch2_vol = 0;
    for (size_t i = 0; i < 32; i++) {
        ch1_vol += HAL::GetAdcValue(1);
        ch2_vol += HAL::GetAdcValue(2);
    }
    drawVolValue((uint32_t)(ch1_vol / 32), (uint32_t)(ch2_vol / 32));
    last_update_time = HAL::Millis();

    HAL::Delay(10);
}

void func_adc_t::stop()
{
}

void func_adc_t::drawVolValue(uint32_t ch1, uint32_t ch2)
{
    HAL::GetDisplay()->fillRect(0, 30, 50, 16, TFT_BLACK);
    HAL::GetDisplay()->fillRect(28, 50, 50, 16, TFT_BLACK);

    char buf[16] = {0};
    sprintf(buf, "%.01fV", ((ch1 / 4095.0f) * 3.3f));
    HAL::GetDisplay()->drawCenterString(buf, 30, 31);

    sprintf(buf, "%.01fV", ((ch2 / 4095.0f) * 3.3));
    HAL::GetDisplay()->drawCenterString(buf, 55, 51);
}
