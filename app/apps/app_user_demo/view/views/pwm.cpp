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

void func_pwm_t::start()
{
    HAL::GetDisplay()->drawPng(pwm_img_list[pwm_mode_index], ~0u, 0, 0);
    HAL::GetDisplay()->setFont(&fonts::efontCN_16_b);
    HAL::GetDisplay()->drawCenterString("F: 1Khz", 90, 52);

    HAL::StartPwm();
    HAL::ChangePwmDuty(pwm_duty);

    drawDuty(pwm_duty);
}

void func_pwm_t::update(bool btn_click)
{
    if (btn_click) {
        pwm_mode_index++;
        if (pwm_mode_index > 3) {
            pwm_mode_index = 0;
        }
        HAL::GetDisplay()->drawPng(pwm_img_list[pwm_mode_index], ~0u, 0, 0);
        HAL::GetDisplay()->drawCenterString("F: 1Khz", 90, 52);
        if (pwm_mode_index != 0) {
            pwm_duty = 0;
        } else {
            pwm_duty = 0xF;
        }

        HAL::ChangePwmDuty(pwm_duty);

        drawDuty(pwm_duty);

        HAL::Delay(10);
    }
}

void func_pwm_t::stop()
{
    HAL::StopPwm();
}

void func_pwm_t::drawDuty(int duty)
{
    HAL::GetDisplay()->fillRect(80, 30, 45, 18, TFT_BLACK);
    HAL::GetDisplay()->setCursor(85, 31);
    HAL::GetDisplay()->setTextColor(TFT_WHITE);
    HAL::GetDisplay()->printf("0x%02X", duty);
}
