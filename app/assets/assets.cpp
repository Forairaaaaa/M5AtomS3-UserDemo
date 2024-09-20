/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "assets.h"
#include "localization/types.h"
#include <mooncake.h>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#ifndef ESP_PLATFORM
#include "fonts/fonts.h"
#include "images/images.h"
#include <iostream>
#include <fstream>
#endif

AssetPool* AssetPool::_asset_pool = nullptr;

AssetPool* AssetPool::Get()
{
    if (_asset_pool == nullptr) _asset_pool = new AssetPool;
    return _asset_pool;
}

StaticAsset_t* AssetPool::getStaticAsset()
{
    if (_data.static_asset == nullptr) {
        spdlog::error("static asset not exsit");
        return nullptr;
    }
    return _data.static_asset;
}

bool AssetPool::injectStaticAsset(StaticAsset_t* asset)
{
    if (_data.static_asset != nullptr) {
        spdlog::error("static asset already exist");
        return false;
    }

    if (asset == nullptr) {
        spdlog::error("invalid static asset ptr");
        return false;
    }

    _data.static_asset = asset;

    // Default local text map
    setLocalTextTo(_data.locale_code);

    spdlog::info("static asset injected");
    return true;
}

void AssetPool::setLocalTextTo(LocaleCode_t code)
{
    // if (code == locale_code_en)
    //     getStaticAsset()->Text.TextEN.getMap(_data.local_text_pool_map);
    // else if (code == locale_code_cn)
    //     getStaticAsset()->Text.TextCN.getMap(_data.local_text_pool_map);
    // else if (code == locale_code_jp)
    //     getStaticAsset()->Text.TextJP.getMap(_data.local_text_pool_map);
}

void AssetPool::setLocaleCode(LocaleCode_t code)
{
    _data.locale_code = code;
    setLocalTextTo(_data.locale_code);
}

void AssetPool::loadFont14(LGFX_SpriteFx* lgfxDevice)
{
    /* ------------------- Load your font by locale code here ------------------- */

    // lgfxDevice->setTextSize(1);
    // if (_data.locale_code == locale_code_en)
    //     lgfxDevice->loadFont(getStaticAsset()->Font.montserrat_semibold_14);
    // else
    //     ...
}

void AssetPool::loadFont16(LGFX_SpriteFx* lgfxDevice)
{
}

void AssetPool::loadFont24(LGFX_SpriteFx* lgfxDevice)
{
}

void AssetPool::loadFont72(LGFX_SpriteFx* lgfxDevice)
{
}

/* -------------------------------------------------------------------------- */
/*                            Static asset generate                           */
/* -------------------------------------------------------------------------- */
#ifdef PLATFORM_BUILD_DESKTOP

/**
 * @brief Copy file into target as binary
 *
 * @param filePath
 * @param target
 * @return true
 * @return false
 */
static bool _copy_file(std::string filePath, uint8_t* target)
{
    spdlog::info("try open {}", filePath);

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        spdlog::error("open failed!", filePath);
        return false;
    }
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    spdlog::info("file binary size {}", file_size);

    // Copy and go
    if (target != nullptr) file.read(reinterpret_cast<char*>(target), file_size);
    file.close();
    return true;
}

/**
 * @brief Convert png image into rgb565 and copy into target as binary
 *
 * @param filePath
 * @param target
 * @return true
 * @return false
 */
#ifdef ENABLE_PNG_CONVERTOR
static bool _copy_png_image(std::string filePath, uint16_t* target)
{
    spdlog::info("try convert: {}", filePath);
    size_t output_length = 0;
    int width = 0, height = 0;

    ImageConversionError result = convertPNGToR5G6B5(filePath.c_str(), target, &output_length, &width, &height);

    if (result != ImageConversionError::Success) {
        spdlog::error("convert failed: {}", static_cast<int>(result));
        return false;
    }

    spdlog::info("ok, image size: {} x {}, array length: {}", width, height, output_length);
    return true;
}
#endif

#include "images/app_user_demo/img_res.h"

StaticAsset_t* AssetPool::CreateStaticAsset()
{
    auto asset_pool = new StaticAsset_t;

    // Copy data
    /* -------------------------------------------------------------------------- */
    /*                                    Fonts                                   */
    /* -------------------------------------------------------------------------- */

    // Copy your font here (or set pointer) like:
    // _copy_file("../../app/assets/fonts/ffff.vlw", asset_pool->Font.ffff);
    // or:
    // std::memcpy(asset_pool->Font.montserrat_semibolditalic_24, montserrat_semibolditalic_24,
    // montserrat_semibolditalic_24_size);

    /* -------------------------------------------------------------------------- */
    /*                                   Images                                   */
    /* -------------------------------------------------------------------------- */

    // Copy your image here (or set pointer) like:
    // _copy_png_image("../../app/assets/images/aaaaa.png", asset_pool->Image.aaaaa);
    // or:
    // std::memcpy(asset_pool->Image.AppLauncher.icon, image_data_icon, image_data_icon_size);

    _copy_file("../../app/assets/images/app_startup_anim/ui_img_logo_r_png.bin",
               asset_pool->Image.AppStartupAnim.logo_r);
    _copy_file("../../app/assets/images/app_user_demo/imu/ui_img_imu_cross_mark_png.bin",
               asset_pool->Image.AppUserDemo.imu_cross_mark);
    _copy_file("../../app/assets/images/app_user_demo/imu/ui_img_imu_dial_png.bin",
               asset_pool->Image.AppUserDemo.imu_dial);
    _copy_file("../../app/assets/images/app_user_demo/imu/ui_img_imu_tilt_ball_png.bin",
               asset_pool->Image.AppUserDemo.imu_tilt_ball);

    std::memcpy(asset_pool->Image.AppUserDemo.wifi_scan_img, wifi_scan_img, sizeof(wifi_scan_img));
    std::memcpy(asset_pool->Image.AppUserDemo.i2c_scan_img, i2c_scan_img, sizeof(i2c_scan_img));
    std::memcpy(asset_pool->Image.AppUserDemo.uart_mon_img, uart_mon_img, sizeof(uart_mon_img));
    std::memcpy(asset_pool->Image.AppUserDemo.uart_mon_01_img, uart_mon_01_img, sizeof(uart_mon_01_img));
    std::memcpy(asset_pool->Image.AppUserDemo.uart_mon_02_img, uart_mon_02_img, sizeof(uart_mon_02_img));
    std::memcpy(asset_pool->Image.AppUserDemo.uart_mon_03_img, uart_mon_03_img, sizeof(uart_mon_03_img));
    std::memcpy(asset_pool->Image.AppUserDemo.uart_mon_04_img, uart_mon_04_img, sizeof(uart_mon_04_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_pwm_img, io_pwm_img, sizeof(io_pwm_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_pwm_01_img, io_pwm_01_img, sizeof(io_pwm_01_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_pwm_02_img, io_pwm_02_img, sizeof(io_pwm_02_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_pwm_03_img, io_pwm_03_img, sizeof(io_pwm_03_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_pwm_04_img, io_pwm_04_img, sizeof(io_pwm_04_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_adc_img, io_adc_img, sizeof(io_adc_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_adc_01_img, io_adc_01_img, sizeof(io_adc_01_img));
    std::memcpy(asset_pool->Image.AppUserDemo.io_test_img, io_test_img, sizeof(io_test_img));
    std::memcpy(asset_pool->Image.AppUserDemo.ir_send_img, ir_send_img, sizeof(ir_send_img));
    std::memcpy(asset_pool->Image.AppUserDemo.ir_send_01_img, ir_send_01_img, sizeof(ir_send_01_img));
    std::memcpy(asset_pool->Image.AppUserDemo.ir_send_02_img, ir_send_02_img, sizeof(ir_send_02_img));
    std::memcpy(asset_pool->Image.AppUserDemo.imu_test_img, imu_test_img, sizeof(imu_test_img));

    return asset_pool;
}

void AssetPool::CreateStaticAssetBin(StaticAsset_t* assetPool)
{
    /* -------------------------------------------------------------------------- */
    /*                                Output to bin                               */
    /* -------------------------------------------------------------------------- */
    std::string bin_path = "AssetPool.bin";

    std::ofstream outFile(bin_path, std::ios::binary);
    if (!outFile) spdlog::error("open {} failed", bin_path);

    outFile.write(reinterpret_cast<const char*>(assetPool), sizeof(StaticAsset_t));
    outFile.close();
    spdlog::info("output asset pool to: {}", bin_path);
}

StaticAsset_t* AssetPool::GetStaticAssetFromBin()
{
    auto asset_pool = new StaticAsset_t;

    // Read from bin
    std::string bin_path = "AssetPool.bin";

    std::ifstream inFile(bin_path, std::ios::binary);
    if (!inFile) spdlog::error("open {} failed", bin_path);

    inFile.read(reinterpret_cast<char*>(asset_pool), sizeof(StaticAsset_t));
    inFile.close();

    spdlog::info("load asset pool from: {}", bin_path);
    return asset_pool;
}
#else

StaticAsset_t* AssetPool::CreateStaticAsset()
{
    auto asset_pool = new StaticAsset_t;

    // ...

    return asset_pool;
}

#endif
