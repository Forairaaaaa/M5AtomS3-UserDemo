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

void func_wifi_t::start()
{
    wifi_scan_count = 0;
    wifi_show_index = 0;
    wifi_scan_done  = false;
    _btn_clicked    = false;

    HAL::StartWifiScan();

    _canvas->setTextWrap(false);
    _canvas->setTextScroll(true);
    _canvas->clear(TFT_BLACK);
    _canvas->setFont(&fonts::efontCN_16);
    _canvas->drawCenterString("Scaning...", _canvas->width() / 2, _canvas->height() / 2 - 24);
    _canvas->setFont(&fonts::efontCN_12);
    needDraw();
}

void func_wifi_t::update(bool btn_click)
{
    if (!wifi_scan_done) {
        if (!HAL::IsWifiScanFinish()) return;

        wifi_scan_done  = true;
        wifi_scan_count = HAL::GetWifiScanResult()->size();
        _canvas->setCursor(0, 0);
        _canvas->clear(TFT_BLACK);
    } else {
        if (btn_click) {
            _btn_clicked = !_btn_clicked;
        }

        if (!_btn_clicked) {
            if (HAL::Millis() - last_time_show > 500) {
                if (wifi_show_index < (wifi_scan_count < 5 ? wifi_scan_count : 5)) {
                    _canvas->printf("%d. %s %d\r\n", wifi_show_index + 1,
                                    (*HAL::GetWifiScanResult())[wifi_show_index].ssid.c_str(),
                                    (*HAL::GetWifiScanResult())[wifi_show_index].rssi);
                    wifi_show_index++;
                } else {
                    _canvas->printf("\r\n\r\n");
                    _canvas->setFont(&fonts::efontCN_14);
                    _canvas->printf("Top 5 list:\r\n");
                    _canvas->setFont(&fonts::efontCN_12);
                    wifi_show_index = 0;
                }
                last_time_show = HAL::Millis();
                needDraw();
            }
        } else {
            // update pause
        }
    }
}

void func_wifi_t::stop()
{
    HAL::FreeWifiScanResult();

    _canvas->setFont(&fonts::Font0);
}
