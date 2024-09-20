/**
 * @file hal_imu.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-07-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal_atom_s3r.h"
#include "../hal_config.h"
#include <cstdint>
#include <cstdio>
#include <mooncake.h>
#include <driver/gpio.h>
#include "../utils/bmi270/src/bmi270.h"
#include "esp32-hal.h"
#include "spdlog/spdlog.h"
#include <smooth_ui_toolkit.h>
// https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi270-ds000.pdf
// https://github.com/boschsensortec/BMI270_SensorAPI
// https://github.com/arduino-libraries/Arduino_BMI270_BMM150

using namespace SmoothUIToolKit;

static BMI270_Class* _imu = nullptr;
static bool _is_bmm150_ok = false;

void HAL_AtomS3R::_imu_init()
{
    spdlog::info("imu init");

    assert(_imu == nullptr);
    _imu = new BMI270_Class();
    if (!_imu->init())
    {
        delete _imu;
        _imu = nullptr;
        // popFatalError("imu init failed");
        spdlog::warn("bmi270 init failed");
    }
    else
    {
        spdlog::info("bmi270 init ok");
    }

    if (!_imu->initAuxBmm150())
    {
        delete _imu;
        _imu = nullptr;
        // popFatalError("imu init bmm150 failed");
        _is_bmm150_ok = false;
        spdlog::warn("bmm150 init failed");
    }
    else
    {
        _is_bmm150_ok = true;
        spdlog::info("bmm150 init ok");
    }

    // Interrupt
    if (_imu != nullptr)
    {
        // _imu->setWristWearWakeup();
        if (!_imu->enableAnyMotionInterrupt())
        {
            // popFatalError("imu enable any motion intterrupt failed");
            spdlog::warn("imu enable any motion intterrupt failed");
        }
        else
        {
            spdlog::info(" enable any motion intterrupt");
        }

        // Pin
        gpio_reset_pin((gpio_num_t)HAL_PIN_IMU_INT);
        gpio_set_direction((gpio_num_t)HAL_PIN_IMU_INT, GPIO_MODE_INPUT);
        gpio_set_pull_mode((gpio_num_t)HAL_PIN_IMU_INT, GPIO_FLOATING);
    }

    // /* -------------------------------------------------------------------------- */
    // /*                                    Test                                    */
    // /* -------------------------------------------------------------------------- */
    // _imu_test();
    // _imu_keep_sending_data();
}

void HAL_AtomS3R::updateImuData()
{
    // _imu->readAcceleration(_data.imu_data.accelX, _data.imu_data.accelY, _data.imu_data.accelZ);
    _imu->readAcceleration(_data.imu_data.accelY, _data.imu_data.accelX, _data.imu_data.accelZ);
    _imu->readGyroscope(_data.imu_data.gyroX, _data.imu_data.gyroY, _data.imu_data.gyroZ);
    _imu->readMagneticField(_data.imu_data.magX, _data.imu_data.magY, _data.imu_data.magZ);

    // Reverse
    _data.imu_data.magX = -_data.imu_data.magX;
    _data.imu_data.magZ = -_data.imu_data.magZ;
}

bool HAL_AtomS3R::getImuInterruptState() { return gpio_get_level((gpio_num_t)HAL_PIN_IMU_INT) == 0; }

bool HAL_AtomS3R::isImuAvailable() { return _imu != nullptr; }

bool HAL_AtomS3R::isImuMagAvailable() { return _is_bmm150_ok; }

void HAL_AtomS3R::updateImuTiltBallOffset()
{
    // spdlog::info("{} {} {}", _data.imu_data.accelX, _data.imu_data.accelY, _data.imu_data.accelZ);

    static float value_limit = 0.7;
    static int offset_limit = 12;
    static float tilt_offset_factor = (float)offset_limit / value_limit;

    _data.imu_data.tiltBallOffsetX = _data.imu_data.accelX * tilt_offset_factor;
    _data.imu_data.tiltBallOffsetY = _data.imu_data.accelY * tilt_offset_factor;

    // Limit
    _data.imu_data.tiltBallOffsetX = SmoothUIToolKit::Clamp(_data.imu_data.tiltBallOffsetX, {-offset_limit, offset_limit});
    _data.imu_data.tiltBallOffsetY = SmoothUIToolKit::Clamp(_data.imu_data.tiltBallOffsetY, {-offset_limit, offset_limit});

    // spdlog::info("{} {}", _data.imu_data.tiltBallOffsetX, _data.imu_data.tiltBallOffsetY);
}

static void _calculate_attitude_yaw(float gyroZ, float deltaTime, float& yaw) { yaw += gyroZ * deltaTime; }

void HAL_AtomS3R::updateImuDialAngle()
{
    static uint32_t time_count = millis();
    static float yaw = 0.0f;

    if (millis() - time_count > 200)
    {
        time_count = millis();
        yaw = 0.0f;
        return;
    }

    _calculate_attitude_yaw(_data.imu_data.gyroZ, millis() - time_count, yaw);
    // spdlog::info("get yaw: {}", yaw);

    _data.imu_data.dialAngle = (int32_t)yaw / 100;

    time_count = millis();
}

void HAL_AtomS3R::_imu_test()
{
    // float ax, ay, az, gx, gy, gz, mx, my, mz;
    int hit = 1;
    while (1)
    {
        feedTheDog();
        // delay(100);
        delay(20);

        hit = gpio_get_level((gpio_num_t)HAL_PIN_IMU_INT);

        updateImuData();
        spdlog::info("{} | {:.1f} {:.1f} {:.1f} | {:.1f} {:.1f} {:.1f} | {:.1f} {:.1f} {:.1f}",
                     hit,
                     getImuData().accelX,
                     getImuData().accelY,
                     getImuData().accelZ,
                     getImuData().gyroX,
                     getImuData().gyroX,
                     getImuData().gyroZ,
                     getImuData().magX,
                     getImuData().magY,
                     getImuData().magZ);

        // _imu->readAcceleration(ax, ay, az);
        // _imu->readGyroscope(gx, gy, gz);
        // _imu->readMagneticField(mx, my, mz);
        // spdlog::info(
        //     "{} | {:.1f} {:.1f} {:.1f} | {:.1f} {:.1f} {:.1f} | {:.1f} {:.1f} {:.1f}", hit, ax, ay, az, gx, gy, gz, mx, my,
        //     mz);

        // feedTheDog();
        // delay(10);
        if (hit == 0)
        {
            spdlog::info("hit");
            delay(500);
        }
    }
}

#include <ArduinoJson.h>

void HAL_AtomS3R::_imu_keep_sending_data()
{
    spdlog::info("start sending imu msg");

    JsonDocument doc;
    std::string json_buffer;

    while (1)
    {
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
