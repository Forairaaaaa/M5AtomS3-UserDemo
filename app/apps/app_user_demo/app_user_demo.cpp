/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "app_user_demo.h"
#include "../../hal/hal.h"
#include "../../assets/assets.h"
#include "../utils/system/system.h"
#include "apps/utils/system/inputs/button/button.h"
#include <cstdint>

using namespace MOONCAKE::APPS;
using namespace SYSTEM::INPUTS;
using namespace SYSTEM::UI;

void AppUserDemo::onResume()
{
    spdlog::info("{} onResume", getAppName());

    _data.func_img_list.resize(7);
    _data.func_img_list[0] = (uint8_t*)AssetPool::GetImage().AppUserDemo.wifi_scan_img;
    _data.func_img_list[1] = (uint8_t*)AssetPool::GetImage().AppUserDemo.i2c_scan_img;
    _data.func_img_list[2] = (uint8_t*)AssetPool::GetImage().AppUserDemo.uart_mon_img;
    _data.func_img_list[3] = (uint8_t*)AssetPool::GetImage().AppUserDemo.io_pwm_img;
    _data.func_img_list[4] = (uint8_t*)AssetPool::GetImage().AppUserDemo.io_adc_img;
    _data.func_img_list[5] = (uint8_t*)AssetPool::GetImage().AppUserDemo.ir_send_img;
    _data.func_img_list[6] = (uint8_t*)AssetPool::GetImage().AppUserDemo.imu_test_img;

    _data.func_list.resize(7);
    _data.func_list[0] = new VIEW::func_wifi_t;
    _data.func_list[1] = new VIEW::func_i2c_t;
    _data.func_list[2] = new VIEW::func_uart_t;
    _data.func_list[3] = new VIEW::func_pwm_t;
    _data.func_list[4] = new VIEW::func_adc_t;
    _data.func_list[5] = new VIEW::func_ir_t;
    _data.func_list[6] = new VIEW::func_imu_t;

    HAL::GetDisplay()->drawPng(_data.func_img_list[_data.func_index], ~0u, 0, 0);
}

void AppUserDemo::onRunning()
{
    Button::Update();

    int btn_state = Button::A()->wasHold() ? 1 : Button::A()->wasClicked() ? 2 : 0;

    if ((btn_state == 2) && !_data.is_entry_func) {
        // next function
        _data.func_index = (VIEW::func_index_t)(_data.func_index + 1);
        if (_data.func_index == VIEW::FUNC_MAX) {
            _data.func_index = VIEW::FUNC_WIFI_SCAN;
        }
        HAL::GetDisplay()->drawPng(_data.func_img_list[_data.func_index], ~0u, 0, 0);
    }

    if (btn_state == 1) {
        if (!_data.is_entry_func) {
            _data.is_entry_func = true;
            // entry function
            _data.func_list[_data.func_index]->entry(HAL::GetCanvas());
        } else {
            _data.is_entry_func = false;

            // leave function
            _data.func_list[_data.func_index]->leave();
            HAL::GetDisplay()->drawPng(_data.func_img_list[_data.func_index], ~0u, 0, 0);
        }
    }

    if (_data.is_entry_func) {
        _data.func_list[_data.func_index]->update(btn_state == 2 ? true : false);
        _data.func_list[_data.func_index]->draw();
    } else {
        HAL::Delay(10);
    }
}

void AppUserDemo::onDestroy()
{
    spdlog::info("{} onDestroy", getAppName());

    for (auto& i : _data.func_list) delete i;
}
