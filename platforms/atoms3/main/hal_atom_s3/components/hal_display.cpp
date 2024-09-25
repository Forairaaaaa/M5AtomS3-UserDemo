/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
// Ref:  https://github.com/lovyan03/LovyanGFX/blob/master/examples/HowToUse/2_user_setting/2_user_setting.ino
#include "../hal_atom_s3.h"
#include "../hal_config.h"
#include <mooncake.h>
// #include <lgfx/v1/panel/Panel_ST7789.hpp>
#include <lgfx/v1/panel/Panel_GC9A01.hpp>
#include "spdlog/spdlog.h"

/* -------------------------------------------------------------------------- */
/*                                    Panel                                   */
/* -------------------------------------------------------------------------- */

class LGFX_AtomS3R : public lgfx::LGFX_Device {
    lgfx::Panel_GC9107 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

public:
    LGFX_AtomS3R(void)
    {
        {
            auto cfg = _bus_instance.config();

            cfg.pin_mosi = HAL_PIN_LCD_MOSI;
            cfg.pin_miso = HAL_PIN_LCD_MISO;
            cfg.pin_sclk = HAL_PIN_LCD_SCLK;
            cfg.pin_dc   = HAL_PIN_LCD_DC;
            // cfg.freq_write = 4000000;
            // cfg.freq_write = 10000000;
            cfg.freq_write = 40000000;
            cfg.spi_host   = SPI2_HOST;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }
        {
            auto cfg = _panel_instance.config();

            cfg.invert    = true;
            cfg.rgb_order = true;
            cfg.pin_cs    = HAL_PIN_LCD_CS;
            cfg.pin_rst   = HAL_PIN_LCD_RST;
            cfg.pin_busy  = HAL_PIN_LCD_BUSY;

            cfg.panel_width      = 128;
            cfg.panel_height     = 128;
            cfg.offset_x         = 0;
            cfg.offset_y         = 32;
            cfg.offset_rotation  = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits  = 1;
            cfg.readable         = true;
            cfg.invert           = false;
            cfg.rgb_order        = false;
            cfg.dlen_16bit       = false;
            cfg.bus_shared       = true;

            _panel_instance.config(cfg);
        }
        {  // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config();  // バックライト設定用の構造体を取得します。

            cfg.pin_bl      = HAL_PIN_LCD_BL;  // バックライトが接続されているピン番号
            cfg.invert      = false;           // バックライトの輝度を反転させる場合 true
            cfg.freq        = 500;             // バックライトのPWM周波数
            cfg.pwm_channel = 7;               // 使用するPWMのチャンネル番号

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
        }

        setPanel(&_panel_instance);
    }
};

void HAL_AtomS3::disp_init()
{
    spdlog::info("display init");

    assert(_data.display == nullptr);
    _data.display = new LGFX_AtomS3R;
    if (!_data.display->init()) {
        delete (_data.display);
        _data.display = nullptr;
        // popFatalError("display init failed");
        spdlog::warn("display init failed");
    }
    // _data.display->setRotation(2);

    assert(_data.canvas == nullptr);
    if (_data.display != nullptr) {
        _data.display->setBrightness(255);
        _data.canvas = new LGFX_SpriteFx(_data.display);
        auto ret     = _data.canvas->createSprite(_data.display->width(), _data.display->height());
        spdlog::info("create canvas: {}", ret);
    }

    /* -------------------------------------------------------------------------- */
    /*                                    Test                                    */
    /* -------------------------------------------------------------------------- */
    // disp_test();
}

void HAL_AtomS3::disp_test()
{
    while (1) {
        feedTheDog();

        spdlog::info("rrr");
        _data.canvas->fillScreen(TFT_RED);
        _data.canvas->pushSprite(0, 0);
        delay(500);

        spdlog::info("ggg");
        _data.canvas->fillScreen(TFT_GREEN);
        _data.canvas->pushSprite(0, 0);
        delay(500);

        spdlog::info("bbb");
        _data.canvas->fillScreen(TFT_BLUE);
        _data.canvas->pushSprite(0, 0);
        delay(500);
    }
}
