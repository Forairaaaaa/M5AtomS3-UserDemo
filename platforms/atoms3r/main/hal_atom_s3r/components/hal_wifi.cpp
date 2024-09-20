/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_atom_s3r.h"
#include "../hal_config.h"
#include <mooncake.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>
#include <vector>
#include <WiFi.h>

bool HAL_AtomS3R::startWifiScan()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.scanNetworks(true);
    return true;
}

bool HAL_AtomS3R::isWifiScanFinish()
{
    return WiFi.scanComplete() >= 0;
}

static std::vector<NETWORK::WifiScanResult_t>* _wifi_scan_result = nullptr;

std::vector<NETWORK::WifiScanResult_t>* HAL_AtomS3R::getWifiScanResult()
{
    if (_wifi_scan_result == nullptr) {
        // Create
        _wifi_scan_result = new std::vector<NETWORK::WifiScanResult_t>;

        // Copy and free
        NETWORK::WifiScanResult_t result_buffer;
        for (int i = 0; i < WiFi.scanComplete(); i++) {
            result_buffer.ssid = WiFi.SSID(i).c_str();
            result_buffer.rssi = WiFi.RSSI(i);
            _wifi_scan_result->push_back(result_buffer);
        }
        WiFi.scanDelete();
    }

    return _wifi_scan_result;
}

void HAL_AtomS3R::freeWifiScanResult()
{
    if (_wifi_scan_result != nullptr) {
        delete _wifi_scan_result;
        _wifi_scan_result = nullptr;
    }
}
