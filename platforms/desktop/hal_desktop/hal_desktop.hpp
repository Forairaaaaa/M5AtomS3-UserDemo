/*
* SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
*
* SPDX-License-Identifier: MIT
*/
#pragma once
#include <cstdint>
#include <mooncake.h>
#include <M5GFX.h>
#include "LGFX_AutoDetect_sdl.hpp"
#include "core/math/math.h"
#include "spdlog/spdlog.h"
#include <hal/hal.h>
#include <cmath>
#include <src/lv_init.h>
#include <src/misc/lv_timer.h>
#include <src/tick/lv_tick.h>
#include <string>
#include <type_traits>
#include <vector>
#include <random>
#include <lvgl.h>
#include <ArduinoJson.h>

class HAL_Desktop : public HAL {
private:
    int _screenWidth;
    int _screenHeight;
    bool _factory_test;

public:
    std::string type() override
    {
        return "AtomS3R";
        // if (_factory_test)
        //     return "AtomS3R";
        // return "Desktop";
    }

    HAL_Desktop(int screenWidth = 240, int screenHeight = 240, bool factoryTest = false)
    {
        _screenWidth  = screenWidth;
        _screenHeight = screenHeight;
        _factory_test = factoryTest;
    }

    void init() override
    {
        // Display
        _data.display = new LGFX(_screenWidth, _screenHeight);
        _data.display->init();

        // Canvas
        _data.canvas = new LGFX_SpriteFx(_data.display);
        _data.canvas->createSprite(_data.display->width(), _data.display->height());

        if (_factory_test) {
            _data.unit_oled = new LGFX(128, 64);
            _data.unit_oled->init();
            _data.unit_oled->setColorDepth(1);
            _data.unit_oled->fillScreen(TFT_WHITE);
        }

        // Some pc window pop up slower?
        lgfx::delay(1500);

        // this->popWarning("404 not found\nasdasd asdfasf");
        // this->popSuccess("404 not found\nasdasd asdfasf");
        // this->popFatalError("404 not found\nasdasd asdfasf");

        _data.config.wifiSsid     = "114514";
        _data.config.wifiPassword = "1919810";

        // Add key mapping
        auto panel = (lgfx::Panel_sdl*)_data.display->getPanel();
        panel->addKeyCodeMapping(SDLK_q, 0);
        panel->addKeyCodeMapping(SDLK_w, 1);
        // panel->addKeyCodeMapping(SDLK_e, 2);
        // panel->addKeyCodeMapping(SDLK_r, 3);
        // panel->addKeyCodeMapping(SDLK_f, 4);
    }

    void canvasUpdate() override
    {
        GetCanvas()->pushSprite(0, 0);
    }

    bool getButton(GAMEPAD::GamePadButton_t button) override
    {
        if (button == GAMEPAD::BTN_A)
            return !lgfx::gpio_in(0);
        else if (button == GAMEPAD::BTN_B)
            return !lgfx::gpio_in(1);
        // else if (button == GAMEPAD::BTN_START)
        //     return !lgfx::gpio_in(2);

        return false;
    }

    /* -------------------------------------------------------------------------- */
    /*                                    Lvgl                                    */
    /* -------------------------------------------------------------------------- */
    static uint32_t _lvgl_tick_cb()
    {
        return HAL::Millis();
    }

    static void _lvgl_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
    {
        int w = (area->x2 - area->x1 + 1);
        int h = (area->y2 - area->y1 + 1);

        HAL::GetDisplay()->startWrite();
        HAL::GetDisplay()->setAddrWindow(area->x1, area->y1, w, h);
        HAL::GetDisplay()->writePixels((lgfx::rgb565_t*)px_map, w * h);
        HAL::GetDisplay()->endWrite();

        lv_disp_flush_ready(disp);
    }

    // https://docs.lvgl.io/master/get-started/quick-overview.html#add-lvgl-into-your-project
    bool lvglInit() override
    {
        spdlog::info("lvgl init");

        lv_init();

        // Tick
        lv_tick_set_cb(_lvgl_tick_cb);

        // Display
        lv_display_t* display = lv_display_create(_data.canvas->width(), _data.canvas->height());
        lv_display_set_buffers(display, _data.canvas->getBuffer(), NULL,
                               sizeof(uint16_t) * _data.canvas->width() * _data.canvas->height(),
                               LV_DISPLAY_RENDER_MODE_FULL);
        lv_display_set_flush_cb(display, _lvgl_disp_flush);

        return true;
    }

    void lvglTimerHandler() override
    {
        lv_timer_handler();
    }

    /* -------------------------------------------------------------------------- */
    /*                                   Encoder                                  */
    /* -------------------------------------------------------------------------- */
    // Simulate encoder by mouse drag
    int encoder_count      = 0;
    int last_encoder_count = 0;
    int touch_start_x      = 0;
    int touch_last_x       = 0;
    bool is_touching       = false;
    int getEncoderCount() override
    {
        if (isTouching()) {
            if (!is_touching) {
                is_touching   = true;
                touch_start_x = getTouchPoint().x;
                return encoder_count;
            }

            if (getTouchPoint().x != touch_last_x) {
                // spdlog::info("{} {} {}", touch_start_x, getTouchPoint().x, encoder_count);
                encoder_count = (getTouchPoint().x - touch_start_x) / 20 + last_encoder_count;
            }
            touch_last_x = getTouchPoint().x;
        } else if (is_touching) {
            is_touching        = false;
            last_encoder_count = encoder_count;
        }

        // // Reverse
        // return -encoder_count;
        // // return encoder_count;

        if (_data.config.reverseEncoder) return encoder_count;
        return -encoder_count;
    }

    void resetEncoderCount(int value) override
    {
        encoder_count = value;
    }

    bool isTouching() override
    {
        lgfx::touch_point_t tp;
        _data.display->getTouch(&tp);
        // spdlog::info("{} {} {}", tp.x, tp.y, tp.size);
        return tp.size != 0;
    }

    TOUCH::Point_t getTouchPoint() override
    {
        lgfx::touch_point_t tp;
        _data.display->getTouch(&tp);
        return {tp.x, tp.y};
    }

    float generateRandomNumber(float v1, float v2)
    {
        std::random_device rd;
        std::default_random_engine generator(rd());
        std::uniform_real_distribution<float> distribution(v1, v2);
        return distribution(generator);
    }

    bool connectWifi(OnLogPageRenderCallback_t onLogPageRender, bool reconnect) override
    {
        onLogPageRender(" connecting wifi..\n", true, true);
        delay(300);
        return true;
    }

    OTA_UPGRADE::OtaInfo_t getLatestFirmwareInfoViaOta(OnLogPageRenderCallback_t onLogPageRender) override
    {
        OTA_UPGRADE::OtaInfo_t info;
        info.firmwareUrl      = "www.114514.com";
        info.latestVersion    = "V6.6.6";
        info.upgradeAvailable = true;
        return info;
    }

    bool upgradeFirmwareViaOta(OnLogPageRenderCallback_t onLogPageRender, const std::string& firmwareUrl) override
    {
        onLogPageRender(" connecting wifi..\n", true, false);
        delay(200);

        onLogPageRender(" upgrading..\n", true, false);
        delay(200);

        for (int i = 0; i < 100; i += 5) {
            std::string log = "<PB>";
            log += std::to_string(i);
            log += "\n";
            onLogPageRender(log, true, false);
            delay(100);
        }

        onLogPageRender(" done\n", true, true);
        delay(200);

        popSuccess("Upgrade done");
        return true;
    }

    bool isWifiScanFinish() override
    {
        return true;
    }

    std::vector<NETWORK::WifiScanResult_t>* getWifiScanResult() override
    {
        static std::vector<NETWORK::WifiScanResult_t> ret = {{"asdasd", -20},      {"0a9fa09gy---sadf", -22},
                                                             {"0asd", -44},        {"0a337400", -66},
                                                             {"0a9fa09g...", -77}, {"~~~", -80}};
        return &ret;
    }

    std::vector<uint8_t>* getI2cScanResult() override
    {
        static std::vector<uint8_t> ret = {0x23, 0x66};
        return &ret;
    }

    // HAL_SERIAL::SerialDesktop* _serial = nullptr;
    // JsonDocument _doc;

    void updateImuData() override
    {
        // if (_serial == nullptr)
        // {
        //     spdlog::info("creat serial");
        //     _serial = new HAL_SERIAL::SerialDesktop;
        //     _serial->init("/dev/ttyACM0");
        // }

        // if (_serial->available())
        // {
        //     auto msg = _serial->readline();
        //     // spdlog::info("get serial msg: {}", msg);

        //     // Parse and copy
        //     deserializeJson(_doc, msg.c_str());
        //     _data.imu_data.accelX = _doc["ax"];
        //     _data.imu_data.accelY = _doc["ay"];
        //     _data.imu_data.accelZ = _doc["az"];
        //     _data.imu_data.gyroX = _doc["gx"];
        //     _data.imu_data.gyroX = _doc["gy"];
        //     _data.imu_data.gyroZ = _doc["gz"];
        //     _data.imu_data.magX = _doc["mx"];
        //     _data.imu_data.magY = _doc["my"];
        //     _data.imu_data.magZ = _doc["mz"];

        //     // spdlog::info("{} {} {} | {} {} {} | {} {} {}",
        //     //              _data.imu_data.magXOffset,
        //     //              _data.imu_data.magYOffset,
        //     //              _data.imu_data.magZOffset,
        //     //              _data.imu_data.magX,
        //     //              _data.imu_data.magY,
        //     //              _data.imu_data.magZ,
        //     //              _data.imu_data.magX - _data.imu_data.magXOffset,
        //     //              _data.imu_data.magY - _data.imu_data.magYOffset,
        //     //              _data.imu_data.magZ - _data.imu_data.magZOffset);
        // }
    }

    void updateImuTiltBallOffset() override
    {
        // spdlog::info("{} {} {}", _data.imu_data.accelX, _data.imu_data.accelY, _data.imu_data.accelZ);

        static float value_limit        = 0.7;
        static int offset_limit         = 12;
        static float tilt_offset_factor = (float)offset_limit / value_limit;

        _data.imu_data.tiltBallOffsetX = _data.imu_data.accelX * tilt_offset_factor;
        _data.imu_data.tiltBallOffsetY = _data.imu_data.accelY * tilt_offset_factor;

        // Limit
        _data.imu_data.tiltBallOffsetX =
            SmoothUIToolKit::Clamp(_data.imu_data.tiltBallOffsetX, {-offset_limit, offset_limit});
        _data.imu_data.tiltBallOffsetY =
            SmoothUIToolKit::Clamp(_data.imu_data.tiltBallOffsetY, {-offset_limit, offset_limit});

        // spdlog::info("{} {}", _data.imu_data.tiltBallOffsetX, _data.imu_data.tiltBallOffsetY);
    }

    void updateImuDialAngle() override
    {
        // spdlog::info("{} {} {}", _data.imu_data.magX, _data.imu_data.magY, _data.imu_data.magZ);

        // auto angle = calculateHeadingAngle(_data.imu_data.magX - _data.imu_data.magXOffset,
        //                                    _data.imu_data.magY - _data.imu_data.magYOffset,
        //                                    _data.imu_data.magZ - _data.imu_data.magZOffset);
        // spdlog::info("angle: {}", angle);
        // _data.imu_data.dialAngle = angle * 10;

        static uint32_t time_count = HAL::Millis();
        static float roll, pitch, yaw = 0.0f;

        if (HAL::Millis() - time_count > 200) {
            time_count = HAL::Millis();
            yaw        = 0.0f;
            return;
        }

        calculateAttitudeAngles(_data.imu_data.accelX, _data.imu_data.accelY, _data.imu_data.accelZ,
                                _data.imu_data.gyroX, _data.imu_data.gyroX, _data.imu_data.gyroZ,
                                HAL::Millis() - time_count, roll, pitch, yaw);
        // spdlog::info("get yaw: {}", yaw);

        _data.imu_data.dialAngle = yaw / 100;

        time_count = HAL::Millis();
    }

    float calculateHeadingAngle(float magX, float magY, float magZ)
    {
        // spdlog::info("{} {} {}", magX, magY, magZ);

        float xyHeading = atan2(magX, magY);
        float zxHeading = atan2(magZ, magX);
        float heading   = xyHeading;

        // Limit
        if (heading < 0) heading += 2 * M_PI;
        if (heading > 2 * M_PI) heading -= 2 * M_PI;

        return (heading * 180 / M_PI);
    }

    void calculateAttitudeAngles(float accelX, float accelY, float accelZ, float gyroX, float gyroY, float gyroZ,
                                 float deltaTime, float& roll, float& pitch, float& yaw)
    {
        // 计算滚转角（Roll）和俯仰角（Pitch）
        roll  = atan2f(accelY, accelZ) * (180.0f / M_PI);
        pitch = atan2f(-accelX, sqrtf(accelY * accelY + accelZ * accelZ)) * (180.0f / M_PI);

        // 计算偏航角（Yaw），假设此处使用陀螺仪积分
        yaw += gyroZ * deltaTime;  // 偏航角通过积分陀螺仪的角速度来估计
    }

    void startImuMagCalibration(uint32_t duration = 10000) override
    {
        spdlog::info("start imu mag calibration");

        float value_x_min = _data.imu_data.magX;
        float value_x_max = _data.imu_data.magX;
        float value_y_min = _data.imu_data.magY;
        float value_y_max = _data.imu_data.magY;
        float value_z_min = _data.imu_data.magZ;
        float value_z_max = _data.imu_data.magZ;

        uint32_t time_count = millis();

        while (1) {
            if (millis() - time_count > duration) break;

            updateImuData();

            if (value_x_min > _data.imu_data.magX)
                value_x_min = _data.imu_data.magX;
            else if (value_x_max < _data.imu_data.magX)
                value_x_max = _data.imu_data.magX;

            if (value_y_min > _data.imu_data.magY)
                value_y_min = _data.imu_data.magY;
            else if (value_y_max < _data.imu_data.magY)
                value_y_max = _data.imu_data.magY;

            if (value_z_min > _data.imu_data.magZ)
                value_z_min = _data.imu_data.magZ;
            else if (value_z_max < _data.imu_data.magZ)
                value_z_max = _data.imu_data.magZ;

            delay(20);
        }

        // _data.imu_data.magXOffset = value_x_min + (value_x_max - value_x_min) / 2;
        // _data.imu_data.magYOffset = value_y_min + (value_y_max - value_y_min) / 2;
        // _data.imu_data.magZOffset = value_z_min + (value_z_max - value_z_min) / 2;

        _data.imu_data.magXOffset = (value_x_max + value_x_min) / 2.0f;
        _data.imu_data.magYOffset = (value_y_max + value_y_min) / 2.0f;
        _data.imu_data.magZOffset = (value_z_max + value_z_min) / 2.0f;

        spdlog::info("get mag offset: {} {} {}", _data.imu_data.magXOffset, _data.imu_data.magYOffset,
                     _data.imu_data.magZOffset);
    }
};
