/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "core/transition2d/transition2d.h"
#include "hal/utils/lgfx_fx/lgfx_fx.h"
#include <cstdint>
#include <mooncake.h>
#include "spdlog/spdlog.h"
#include <assets/assets.h>
#include <smooth_ui_toolkit.h>

namespace VIEW {

#define LAYOUT_OFFSET_Y 30

typedef enum {
    FUNC_WIFI_SCAN,
    FUNC_I2C_SCAN,
    FUNC_UART_MON,
    FUNC_PWM_TEST,
    FUNC_ADC_TEST,
    FUNC_IR_SEND,
    FUNC_IMU_TEST,
    FUNC_MAX
} func_index_t;

class func_base_t {
public:
    LGFX_SpriteFx* _canvas;
    bool _draw_flag;
    bool _btn_clicked;

public:
    virtual ~func_base_t() = default;

    void entry(LGFX_SpriteFx* canvas)
    {
        _canvas = canvas;
        _canvas->fillRect(0, 0, _canvas->width(), _canvas->height(), TFT_BLACK);
        needDraw();
        start();
    };

    virtual void start()
    {
    }
    virtual void update(bool btn_click)
    {
    }
    virtual void stop()
    {
    }

    void needDraw()
    {
        _draw_flag = true;
    }

    virtual void draw()
    {
        if (_draw_flag) {
            // spdlog::info("draw");
            _draw_flag = false;
            _canvas->pushSprite(0, LAYOUT_OFFSET_Y);
        }
    };

    void leave(void)
    {
        stop();
    }
};

class func_wifi_t : public func_base_t {
    time_t last_time_show   = 0;
    int16_t wifi_scan_count = 0;
    int16_t wifi_show_index = 0;
    bool wifi_scan_done     = false;

    void start() override;
    void update(bool btn_click) override;
    void stop() override;
};

class func_i2c_t : public func_base_t {
    uint8_t addr_list[6] = {};
    uint8_t device_count = 0;

    void start() override;
    void update(bool btn_click) override;
    void stop() override;
    void draw_form();
};

class func_uart_t : public func_base_t {
    const unsigned char* uart_mon_img_list[4] = {
        AssetPool::GetImage().AppUserDemo.uart_mon_01_img, AssetPool::GetImage().AppUserDemo.uart_mon_02_img,
        AssetPool::GetImage().AppUserDemo.uart_mon_03_img, AssetPool::GetImage().AppUserDemo.uart_mon_04_img};
    const uint8_t uart_io_list[4][2] = {{2, 1}, {2, 1}, {1, 2}, {1, 2}};
    const uint32_t uart_baud_list[4] = {9600, 115200, 9600, 115200};
    uint8_t uart_mon_mode_index      = 0;

    void start() override;
    void update(bool btn_click) override;
    void stop() override;
    void draw() override
    {
    }
};

class func_pwm_t : public func_base_t {
    const unsigned char* pwm_img_list[4] = {
        AssetPool::GetImage().AppUserDemo.io_pwm_01_img, AssetPool::GetImage().AppUserDemo.io_pwm_02_img,
        AssetPool::GetImage().AppUserDemo.io_pwm_03_img, AssetPool::GetImage().AppUserDemo.io_pwm_04_img};
    uint8_t pwm_mode_index = 0;
    uint8_t pwm_duty       = 128;

    void start() override;
    void update(bool btn_click) override;
    void stop() override;
    void draw() override
    {
    }
    void drawDuty(int duty);
};

class func_adc_t : public func_base_t {
    time_t last_update_time = 0;
    uint32_t ch1_vol        = 0;
    uint32_t ch2_vol        = 0;

    void start() override;
    void update(bool btn_click) override;
    void stop() override;
    void draw() override
    {
    }
    void drawVolValue(uint32_t ch1, uint32_t ch2);
};

class func_ir_t : public func_base_t {
    const unsigned char* ir_img_list[2] = {AssetPool::GetImage().AppUserDemo.ir_send_01_img,
                                           AssetPool::GetImage().AppUserDemo.ir_send_02_img};
    uint8_t ir_send_mode_index          = 0;
    uint32_t ir_cmd                     = 0;
    time_t ir_last_send_time            = 0;

    void start() override;
    void update(bool btn_click) override;
    void stop() override;
    void draw() override
    {
    }
    void drawIrData(uint32_t _ir_cmd);
};

class func_imu_t : public func_base_t {
    void start() override;
    void update(bool btn_click) override;
    void stop() override;
    void draw() override
    {
    }
};
}  // namespace VIEW
