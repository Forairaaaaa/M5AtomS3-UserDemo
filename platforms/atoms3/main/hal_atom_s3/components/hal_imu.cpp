/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_atom_s3.h"
#include "../hal_config.h"
#include <cstdint>
#include <cstdio>
#include <mooncake.h>
#include <driver/gpio.h>
#include "esp32-hal.h"
#include "spdlog/spdlog.h"
#include <smooth_ui_toolkit.h>
#include <utility/imu/MPU6886_Class.hpp>
// https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi270-ds000.pdf
// https://github.com/boschsensortec/BMI270_SensorAPI
// https://github.com/arduino-libraries/Arduino_BMI270_BMM150

using namespace SmoothUIToolKit;

static m5::MPU6886_Class* _imu = nullptr;
static bool _is_bmm150_ok      = false;

void HAL_AtomS3::imu_init()
{
    spdlog::info("imu init");

    assert(_imu == nullptr);
    _imu = new m5::MPU6886_Class();
    if (!_imu->begin()) {
        delete _imu;
        _imu = nullptr;
        // popFatalError("imu init failed");
        spdlog::warn("bmi270 init failed");
    } else {
        spdlog::info("bmi270 init ok");
    }

    // /* -------------------------------------------------------------------------- */
    // /*                                    Test                                    */
    // /* -------------------------------------------------------------------------- */
    // _imu_test();
    // _imu_keep_sending_data();
}

void HAL_AtomS3::updateImuData()
{
    static m5::IMU_Base::imu_raw_data_t raw_data;

    _imu->getImuRawData(&raw_data);

    static float resolution1 = (8.0f / 32768.0f) * (1.0f / 65536.0f);
    _data.imu_data.accelX    = ((int32_t)raw_data.accel.x << 16) * resolution1;
    _data.imu_data.accelY    = ((int32_t)raw_data.accel.y << 16) * resolution1;
    _data.imu_data.accelZ    = ((int32_t)raw_data.accel.z << 16) * resolution1;

    static float resolution2 = (2000.0f / 32768.0f) * (1.0f / 65536.0f);
    _data.imu_data.gyroX     = ((int32_t)raw_data.gyro.x << 16) * resolution2;
    _data.imu_data.gyroY     = ((int32_t)raw_data.gyro.y << 16) * resolution2;
    _data.imu_data.gyroZ     = ((int32_t)raw_data.gyro.z << 16) * resolution2;

    // Reverse
    _data.imu_data.accelY = -_data.imu_data.accelY;
}

bool HAL_AtomS3::getImuInterruptState()
{
    return gpio_get_level((gpio_num_t)HAL_PIN_IMU_INT) == 0;
}

bool HAL_AtomS3::isImuAvailable()
{
    return _imu != nullptr;
}

bool HAL_AtomS3::isImuMagAvailable()
{
    return _is_bmm150_ok;
}

void HAL_AtomS3::updateImuTiltBallOffset()
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

static void _calculate_attitude_yaw(float gyroZ, float deltaTime, float& yaw)
{
    yaw += gyroZ * deltaTime;
}

void HAL_AtomS3::updateImuDialAngle()
{
    static uint32_t time_count = millis();
    static float yaw           = 0.0f;

    if (millis() - time_count > 200) {
        time_count = millis();
        yaw        = 0.0f;
        return;
    }

    _calculate_attitude_yaw(_data.imu_data.gyroZ, millis() - time_count, yaw);
    // spdlog::info("get yaw: {}", yaw);

    _data.imu_data.dialAngle = (int32_t)yaw / 100;

    time_count = millis();
}

void HAL_AtomS3::imu_test()
{
    // float ax, ay, az, gx, gy, gz, mx, my, mz;
    int hit = 1;
    while (1) {
        feedTheDog();
        // delay(100);
        delay(20);

        // hit = gpio_get_level((gpio_num_t)HAL_PIN_IMU_INT);
        updateImuData();
        spdlog::info("{} | {:.1f} {:.1f} {:.1f} | {:.1f} {:.1f} {:.1f} | {:.1f} {:.1f} {:.1f}", hit,
                     getImuData().accelX, getImuData().accelY, getImuData().accelZ, getImuData().gyroX,
                     getImuData().gyroX, getImuData().gyroZ, getImuData().magX, getImuData().magY, getImuData().magZ);
    }
}

#include <ArduinoJson.h>

void HAL_AtomS3::imu_keep_sending_data()
{
    spdlog::info("start sending imu msg");

    JsonDocument doc;
    std::string json_buffer;

    while (1) {
        delay(5);
        feedTheDog();

        updateImuData();

        // Encode json
        doc["ax"] = getImuData().accelX;
        doc["ay"] = getImuData().accelY;
        doc["az"] = getImuData().accelZ;
        doc["gx"] = getImuData().gyroX;
        doc["gy"] = getImuData().gyroY;
        doc["gz"] = getImuData().gyroZ;
        doc["mx"] = getImuData().magX;
        doc["my"] = getImuData().magY;
        doc["mz"] = getImuData().magZ;

        serializeJson(doc, json_buffer);

        printf("%s\n", json_buffer.c_str());
    }
}
