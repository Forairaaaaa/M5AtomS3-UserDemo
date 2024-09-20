/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <cstdint>
#include <string>
#include <functional>
#include <smooth_ui_toolkit.h>
#include <thread>
#include <mutex>

/* -------------------------------------------------------------------------- */
/*                               Gamepad button                               */
/* -------------------------------------------------------------------------- */
namespace GAMEPAD {
enum GamePadButton_t {
    BTN_START = 0,
    BTN_SELECT,
    BTN_UP,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_DOWN,
    BTN_X,
    BTN_Y,
    BTN_A,
    BTN_B,
    BTN_LEFT_STICK,
    GAMEPAD_BUTTON_NUM,
};
}  // namespace GAMEPAD

/* -------------------------------------------------------------------------- */
/*                                     IMU                                    */
/* -------------------------------------------------------------------------- */
namespace IMU {
struct ImuData_t {
    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;

    float gyroX = 0.0f;
    float gyroY = 0.0f;
    float gyroZ = 0.0f;

    float magX = 0.0f;
    float magY = 0.0f;
    float magZ = 0.0f;

    float magXOffset = 0.0f;
    float magYOffset = 0.0f;
    float magZOffset = 0.0f;

    int32_t tiltBallOffsetX = 0;
    int32_t tiltBallOffsetY = 0;
    int32_t dialAngle       = 0;
};
}  // namespace IMU

/* -------------------------------------------------------------------------- */
/*                                  Touchpad                                  */
/* -------------------------------------------------------------------------- */
namespace TOUCH {
/**
 * @brief Touch point
 *
 */
struct Point_t {
    int x;
    int y;
};
}  // namespace TOUCH

/* -------------------------------------------------------------------------- */
/*                                 Locale code                                */
/* -------------------------------------------------------------------------- */
enum LocaleCode_t {
    locale_code_en = 0,
    locale_code_cn,
    locale_code_jp,
};

/* -------------------------------------------------------------------------- */
/*                                System config                               */
/* -------------------------------------------------------------------------- */
namespace CONFIG {
// Default config
struct SystemConfig_t {
    uint8_t brightness       = 255;
    uint8_t orientation      = 0;
    std::string wifiSsid     = "";
    std::string wifiPassword = "";
    bool beepOn              = true;
    bool reverseEncoder      = false;
    LocaleCode_t localeCode  = locale_code_en;
};
}  // namespace CONFIG

/* -------------------------------------------------------------------------- */
/*                                   Network                                  */
/* -------------------------------------------------------------------------- */
namespace NETWORK {
struct WifiScanResult_t {
    std::string ssid;
    int rssi = 0;
};
}  // namespace NETWORK

/* -------------------------------------------------------------------------- */
/*                                  OTA info                                  */
/* -------------------------------------------------------------------------- */
namespace OTA_UPGRADE {
struct OtaInfo_t {
    bool upgradeAvailable = false;
    bool getInfoFailed    = false;
    std::string latestVersion;
    std::string firmwareUrl;
};
};  // namespace OTA_UPGRADE

/* -------------------------------------------------------------------------- */
/*                                Factory Test                                */
/* -------------------------------------------------------------------------- */
namespace FACTORY_TEST {
class WifiTestResult_t {
public:
    struct Data_t {
        std::mutex mutex;
        bool is_done                = false;
        bool is_passed              = false;
        int32_t best_rssi           = 0;
        std::string test_result     = "";
        std::string target_ssid     = "";
        std::string target_password = "";
    };
    Data_t _data;

    Data_t& Borrow()
    {
        _data.mutex.lock();
        return _data;
    }

    void Return()
    {
        _data.mutex.unlock();
    }

    Data_t& Data()
    {
        return _data;
    }
};

};  // namespace FACTORY_TEST

/* -------------------------------------------------------------------------- */
/*                                    MISC                                    */
/* -------------------------------------------------------------------------- */
typedef std::function<void(const std::string& log, bool pushScreen, bool clear)> OnLogPageRenderCallback_t;
