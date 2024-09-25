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
#include <string>
#include <mooncake.h>

using namespace VIEW;

void func_ir_t::start()
{
    HAL::GetDisplay()->drawPng(ir_img_list[ir_send_mode_index], ~0u, 0, 0);
    HAL::GetDisplay()->setFont(&fonts::efontCN_16_b);
    HAL::GetDisplay()->setTextColor(TFT_PURPLE);
}

void func_ir_t::update(bool btn_click)
{
    if (btn_click) {
        ir_send_mode_index++;
        if (ir_send_mode_index > 1) {
            ir_send_mode_index = 0;
        }
        HAL::GetDisplay()->drawPng(ir_img_list[ir_send_mode_index], ~0u, 0, 0);
    }

    // USB => IR
    if (ir_send_mode_index == 0) {
        uint8_t cmd = 0;
        if (HAL::GetIrCmdFromSerial(cmd)) {
            HAL::IrSendCmd(cmd);
            drawIrData((uint32_t)cmd);
        }
    } else {
        if (HAL::Millis() - ir_last_send_time > 1000) {
            ir_cmd++;
            // TO HAL
            // ir_tx_send(ir_cmd);
            HAL::IrSendCmd(ir_cmd);
            drawIrData(ir_cmd);
            ir_last_send_time = HAL::Millis();
        }
    }

    HAL::Delay(10);
}

void func_ir_t::stop()
{
    HAL::GetDisplay()->setTextColor(TFT_WHITE);
}

void func_ir_t::drawIrData(uint32_t _ir_cmd)
{
    HAL::GetDisplay()->fillRect(25, 105, 75, 20, TFT_WHITE);
    std::string string_buffer = fmt::format("0x{:02X}", _ir_cmd);
    HAL::GetDisplay()->drawCenterString(string_buffer.c_str(), 64, 107);
}
