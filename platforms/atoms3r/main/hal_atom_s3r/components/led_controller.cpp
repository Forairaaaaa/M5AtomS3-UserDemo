/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_atom_s3r.h"
#include "../hal_config.h"
#include "../utils/lp5562/lp5562.h"
#include <mooncake.h>
#include "spdlog/spdlog.h"

static LP5562_Class* _led_controller = nullptr;
LP5562_Class* __get_led_controller()
{
    return _led_controller;
}

void HAL_AtomS3R::led_controller_init()
{
    spdlog::info("led controller init");

    assert(_led_controller == nullptr);
    _led_controller = new LP5562_Class;
    if (!_led_controller->begin()) {
        delete _led_controller;
        _led_controller = nullptr;
        // popFatalError("rgb controller init failed");
        spdlog::warn("rgb controller init failed");
    } else {
        spdlog::info("ok");
    }

    if (_led_controller == nullptr) return;

    // PWM clock frequency 558 Hz
    auto data = _led_controller->readRegister8(0x08);
    data      = data | 0B01000000;
    // data = data & 0B10111111;
    _led_controller->writeRegister8(0x08, data);

    // /* -------------------------------------------------------------------------- */
    // /*                                    Test                                    */
    // /* -------------------------------------------------------------------------- */
    // while (1)
    // {
    //     feedTheDog();

    //     spdlog::info("000");
    //     _led_controller->setBrightness(3, 0);
    //     delay(500);

    //     spdlog::info("127");
    //     _led_controller->setBrightness(3, 127);
    //     delay(500);

    //     spdlog::info("255");
    //     _led_controller->setBrightness(3, 255);
    //     delay(500);
    // }
}

bool HAL_AtomS3R::isLedControllerAvailable()
{
    return _led_controller != nullptr;
}
