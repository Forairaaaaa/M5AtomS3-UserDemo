/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
// Ref:  https://github.com/lovyan03/LovyanGFX/blob/master/examples/HowToUse/2_user_setting/2_user_setting.ino
#include "../hal_atom_s3r.h"
#include "../hal_config.h"
#include <mooncake.h>
// #include <lgfx/v1/panel/Panel_ST7789.hpp>
#include <lgfx/v1/panel/Panel_GC9A01.hpp>
#include "../utils/lp5562/lp5562.h"
#include "spdlog/spdlog.h"
#include <M5UnitOLED.h>

/* -------------------------------------------------------------------------- */
/*                                  Backlight                                 */
/* -------------------------------------------------------------------------- */
LP5562_Class* __get_led_controller();

class Light_AtomS3R : public lgfx::ILight {
public:
    bool init(uint8_t brightness) override
    {
        spdlog::info("lcd backlight init");
        // assert(__get_led_controller() != nullptr);
        return true;
    }

    void setBrightness(uint8_t brightness) override
    {
        if (__get_led_controller() == nullptr) {
            spdlog::warn("led controller is not available");
            return;
        }
        __get_led_controller()->setBrightness(3, brightness);
    }
};

/* -------------------------------------------------------------------------- */
/*                                    Panel                                   */
/* -------------------------------------------------------------------------- */

class LGFX_AtomS3R : public lgfx::LGFX_Device {
    lgfx::Panel_GC9107 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    Light_AtomS3R _light_instance;

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
            _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
        }

        setPanel(&_panel_instance);
    }
};

void HAL_AtomS3R::disp_init()
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

    assert(_data.canvas == nullptr);
    if (_data.display != nullptr) {
        _data.canvas = new LGFX_SpriteFx(_data.display);
        _data.canvas->createSprite(_data.display->width(), _data.display->height());
    }

    // Unit oled for factory test
    assert(_data.unit_oled == nullptr);
    _data.unit_oled = new M5UnitOLED(HAL_PIN_I2C_EXTER_SDA, HAL_PIN_I2C_EXTER_SCL, 400000, 1);
    if (!_data.unit_oled->init()) {
        delete _data.unit_oled;
        _data.unit_oled = nullptr;
        // popFatalError("unit oled init failed");
        spdlog::warn("unit oled init failed");
    } else {
        _data.unit_oled->setRotation(1);
        _data.unit_oled->fillScreen(TFT_WHITE);
    }

    /* -------------------------------------------------------------------------- */
    /*                                    Test                                    */
    /* -------------------------------------------------------------------------- */
    // _disp_test();
}

void HAL_AtomS3R::disp_test()
{
    while (1) {
        feedTheDog();

        spdlog::info("rrr");
        _data.display->fillScreen(TFT_RED);
        delay(500);

        spdlog::info("ggg");
        _data.display->fillScreen(TFT_GREEN);
        delay(500);

        spdlog::info("bbb");
        _data.display->fillScreen(TFT_BLUE);
        delay(500);

        spdlog::info("fff");
        _data.display->fillScreen(TFT_WHITE);
        _data.display->drawRect(0, 0, _data.display->width(), _data.display->height(), TFT_RED);
        delay(3000);
    }
}
