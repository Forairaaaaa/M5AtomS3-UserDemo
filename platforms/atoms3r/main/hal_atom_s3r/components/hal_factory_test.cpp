/**
 * @file hal_factory_test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-07-31
 *
 * @copyright Copyright (c) 2024
 *
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
#include <esp_psram.h>

/* -------------------------------------------------------------------------- */
/*                                    Wifi                                    */
/* -------------------------------------------------------------------------- */
// .vscode/settings.json:
// "cmake.environment": {
// "FACTORY_TEST_WIFI_SSID": "114514",
// "FACTORY_TEST_WIFI_PASSWORD": "2333"
// }
// or:
// export FACTORY_TEST_WIFI_SSID="114514"
// export FACTORY_TEST_WIFI_PASSWORD="2333"

#define WIFI_PASS_RSSI_TH -50

static FACTORY_TEST::WifiTestResult_t _wifi_test_result;

static void _wifi_test_daemon(void* param)
{
    spdlog::info("start wifi test daemon");

    // Reset
    _wifi_test_result.Borrow();
    _wifi_test_result.Data().is_done = false;
    _wifi_test_result.Data().is_passed = false;
    _wifi_test_result.Data().best_rssi = 0;
    _wifi_test_result.Data().test_result = "";
    _wifi_test_result.Data().target_ssid = FACTORY_TEST_WIFI_SSID;
    _wifi_test_result.Data().target_password = FACTORY_TEST_WIFI_PASSWORD;
    _wifi_test_result.Return();

    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    /* -------------------------------- Scan test ------------------------------- */
    spdlog::info("[wifi test] start wifi scan");
    int n = WiFi.scanNetworks();
    _wifi_test_result.Borrow();
    if (n == 0)
    {
        WiFi.scanDelete();

        spdlog::info("[wifi test] scan failed");

        _wifi_test_result.Data().test_result = "扫描失败";
        _wifi_test_result.Data().is_done = true;
        _wifi_test_result.Data().is_passed = false;
        _wifi_test_result.Return();

        vTaskDelete(NULL);
        vTaskDelay(66666);
        return;
    }
    else
    {
        _wifi_test_result.Data().best_rssi = WiFi.RSSI(0);
        for (int i = 0; i < n; ++i)
        {
            if (WiFi.RSSI(i) > _wifi_test_result.Data().best_rssi)
                _wifi_test_result.Data().best_rssi = WiFi.RSSI(i);
        }
        spdlog::info("[wifi test] get best rssi: {} in {} wifis", _wifi_test_result.Data().best_rssi, n);

        // If not pass
        if (_wifi_test_result.Data().best_rssi < WIFI_PASS_RSSI_TH)
        {
            WiFi.scanDelete();

            spdlog::info("[wifi test] lower than threshold: {}", WIFI_PASS_RSSI_TH);

            _wifi_test_result.Data().test_result = "RSSI值低: ";
            _wifi_test_result.Data().test_result += std::to_string(_wifi_test_result.Data().best_rssi);

            _wifi_test_result.Data().is_done = true;
            _wifi_test_result.Data().is_passed = false;
            _wifi_test_result.Return();

            vTaskDelete(NULL);
            vTaskDelay(66666);
            return;
        }
    }
    _wifi_test_result.Return();

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    /* ----------------------------- Connection test ---------------------------- */
    spdlog::info("[wifi test] try connect\nssid: {}\npass: {}", FACTORY_TEST_WIFI_SSID, FACTORY_TEST_WIFI_PASSWORD);
    WiFi.begin(FACTORY_TEST_WIFI_SSID, FACTORY_TEST_WIFI_PASSWORD);

    /* Wait for the connection */
    uint32_t time_conut = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);

        // Check timeout
        if ((millis() - time_conut) > 10000)
        {
            spdlog::info("[wifi test] connect time out: {}ms", 10000);

            _wifi_test_result.Borrow();
            _wifi_test_result.Data().test_result = "连接超时";
            _wifi_test_result.Data().is_done = true;
            _wifi_test_result.Data().is_passed = false;
            _wifi_test_result.Return();

            vTaskDelete(NULL);
            vTaskDelay(66666);
            return;
        }
    }

    // Get ip
    String ip = WiFi.localIP().toString();
    spdlog::info("[wifi test] get ip: {}", ip.c_str());

    /* ---------------------------------- Done ---------------------------------- */
    _wifi_test_result.Borrow();
    _wifi_test_result.Data().test_result = "测试通过 IP:";
    _wifi_test_result.Data().test_result += ip.c_str();
    _wifi_test_result.Data().is_done = true;
    _wifi_test_result.Data().is_passed = true;
    _wifi_test_result.Return();
    delay(1000);
    vTaskDelete(NULL);
}

void HAL_AtomS3R::startWifiFactoryTestDaemon()
{
    spdlog::info("create mbus test daemon");
    xTaskCreate(_wifi_test_daemon, "wifi", 4000, NULL, 6, NULL);
}

FACTORY_TEST::WifiTestResult_t* HAL_AtomS3R::getWifiFactoryTestResult() { return &_wifi_test_result; }

void HAL_AtomS3R::startBleFactoryTestDaemon() {}

/* -------------------------------------------------------------------------- */
/*                                    Mbus                                    */
/* -------------------------------------------------------------------------- */
static std::vector<int> _mbus_pin_list = {5, 6, 7, 8, 38, 39};

static void _mbus_test_daemon(void* param)
{
    spdlog::info("start mbus test daemon");

    // Setup
    for (auto& i : _mbus_pin_list)
    {
        gpio_reset_pin((gpio_num_t)i);
        gpio_set_direction((gpio_num_t)i, GPIO_MODE_INPUT_OUTPUT);
        gpio_set_pull_mode((gpio_num_t)i, GPIO_PULLUP_PULLDOWN);
        gpio_set_level((gpio_num_t)i, 0);
    }

    while (1)
    {
        for (auto& i : _mbus_pin_list)
        {
            // spdlog::info("{} {}", i, gpio_get_level((gpio_num_t)i) == 0 ? 1 : 0);
            gpio_set_level((gpio_num_t)i, gpio_get_level((gpio_num_t)i) == 0 ? 1 : 0);
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        spdlog::info("[mbus daemon] im ok");
    }

    vTaskDelete(NULL);
}

void HAL_AtomS3R::startMbusFactoryTestDaemon()
{
    spdlog::info("create mbus test daemon");
    xTaskCreate(_mbus_test_daemon, "io", 4000, NULL, 5, NULL);
}

size_t HAL_AtomS3R::getPsramSize() { return esp_psram_get_size(); }
