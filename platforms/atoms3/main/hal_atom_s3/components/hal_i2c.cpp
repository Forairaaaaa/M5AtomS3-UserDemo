/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_atom_s3.h"
#include "../hal_config.h"
#include "esp32-hal.h"
#include <mooncake.h>
#include <utility/I2C_Class.hpp>


static void i2c_scan(m5::I2C_Class& i2c_class)
{
    spdlog::info("start scan:");
    bool scan_list[120];
    i2c_class.scanID(scan_list);
    uint8_t device_num = 0;
    for (int i = 8; i < 0x78; i++) {
        if (scan_list[i]) {
            device_num++;
            spdlog::info("get 0x{0:x}", i);
        }
    }
    spdlog::info("device num: {}", device_num);
}

void HAL_AtomS3::i2c_init()
{
    spdlog::info("i2c bus init");

    /* -------------------------------- Internal -------------------------------- */
    spdlog::info("internal bus:");
    m5::In_I2C.begin(I2C_NUM_0, HAL_PIN_I2C_INTER_SDA, HAL_PIN_I2C_INTER_SCL);
    // _i2c_scan(m5::In_I2C);

    /* -------------------------------- External -------------------------------- */
    spdlog::info("external bus:");
    m5::Ex_I2C.begin(I2C_NUM_1, HAL_PIN_I2C_EXTER_SDA, HAL_PIN_I2C_EXTER_SCL);
    // _i2c_scan(m5::Ex_I2C);
}

static std::vector<uint8_t>* _i2c_scan_result = nullptr;

bool HAL_AtomS3::startI2CScan()
{
    if (_i2c_scan_result == nullptr) _i2c_scan_result = new std::vector<uint8_t>;

    bool scan_list[120];
    m5::Ex_I2C.scanID(scan_list);

    _i2c_scan_result->clear();
    for (int i = 8; i < 0x78; i++) {
        if (scan_list[i]) _i2c_scan_result->push_back(i);
    }

    return true;
}

std::vector<uint8_t>* HAL_AtomS3::getI2cScanResult()
{
    return _i2c_scan_result;
}

void HAL_AtomS3::freeI2cScanResult()
{
    if (_i2c_scan_result != nullptr) {
        delete _i2c_scan_result;
        _i2c_scan_result = nullptr;
    }
}
