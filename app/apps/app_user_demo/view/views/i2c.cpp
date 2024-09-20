/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../view.h"
#include <hal/hal.h>
#include <assets/assets.h>
#include <shared/shared.h>
#include <mooncake.h>
#include "spdlog/spdlog.h"

using namespace VIEW;

void func_i2c_t::start()
{
    _canvas->setTextWrap(false);
    _canvas->setTextScroll(true);
    _canvas->clear(TFT_BLACK);
    _canvas->setFont(&fonts::efontCN_16);
    _canvas->drawCenterString("Scaning...", _canvas->width() / 2, _canvas->height() / 2 - 12);
    needDraw();
    _btn_clicked = false;
}

void func_i2c_t::update(bool btn_click)
{
    if (btn_click) {
        _btn_clicked = !_btn_clicked;
        _canvas->clear(TFT_BLACK);
        _canvas->setTextSize(1);
        _canvas->setTextColor(TFT_WHITE);
        _canvas->setFont(&fonts::efontCN_16);
        _canvas->drawCenterString("Scaning...", _canvas->width() / 2, _canvas->height() / 2 - 12);
        needDraw();
        draw();
    }

    if (!_btn_clicked) {
        _btn_clicked = true;
        needDraw();

        uint8_t address = 0;
        device_count    = 0;
        memset(addr_list, 0, sizeof(addr_list));

        HAL::StartI2CScan();
        device_count = HAL::GetI2cScanResult()->size();
        if (device_count > 6) device_count = 6;
        for (int i = 0; i < device_count; i++) addr_list[i] = (*HAL::GetI2cScanResult())[i];

        _canvas->clear();
        if (device_count == 0) {
            _canvas->setFont(&fonts::efontCN_24);
            _canvas->setTextSize(1);
            _canvas->setTextColor(TFT_RED);
            _canvas->drawCenterString("Not found", 64, _canvas->height() / 2 - 24);
            return;
        }

        std::string addr_buf;
        _canvas->setFont(&fonts::efontCN_16);
        draw_form();
        for (size_t i = 0; i < device_count; i++) {
            // sprintf(addr_buf, "%d. 0x%02X", i + 1, addr_list[i]);
            addr_buf = fmt::format("{}. 0x{:02X}", i + 1, addr_list[i]);

            _canvas->setTextColor(
                (HAL::RandomInt(0, 255) << 16 | HAL::RandomInt(0, 255) << 8 | HAL::RandomInt(0, 255)));
            _canvas->drawCenterString(addr_buf.c_str(), (32 * i > 2 ? 1 : 0) + 32, (32 * (i % 3)) + 8);
        }
    }

    HAL::Delay(10);
}

void func_i2c_t::stop()
{
    HAL::FreeI2cScanResult();
    _canvas->setTextColor(TFT_WHITE);
}

void func_i2c_t::draw_form()
{
    _canvas->drawFastHLine(0, (_canvas->height() - LAYOUT_OFFSET_Y) / 3 * 0 + 1, _canvas->width(), TFT_WHITE);
    _canvas->drawFastHLine(0, (_canvas->height() - LAYOUT_OFFSET_Y) / 3 * 1, _canvas->width(), TFT_WHITE);
    _canvas->drawFastHLine(0, (_canvas->height() - LAYOUT_OFFSET_Y) / 3 * 2, _canvas->width(), TFT_WHITE);
    _canvas->drawFastHLine(0, (_canvas->height() - LAYOUT_OFFSET_Y) / 3 * 3, _canvas->width(), TFT_WHITE);
    _canvas->drawFastVLine(_canvas->width() / 2 * 0, 1, (_canvas->height() - LAYOUT_OFFSET_Y) - 2, TFT_WHITE);
    _canvas->drawFastVLine(_canvas->width() / 2 * 1, 1, (_canvas->height() - LAYOUT_OFFSET_Y) - 2, TFT_WHITE);
    _canvas->drawFastVLine(_canvas->width() / 2 * 2 - 1, 1, (_canvas->height() - LAYOUT_OFFSET_Y) - 2, TFT_WHITE);
}
