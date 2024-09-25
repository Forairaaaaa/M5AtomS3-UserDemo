/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <cstdint>

/* -------------------------------------------------------------------------- */
/*                                   Images                                   */
/* -------------------------------------------------------------------------- */
struct ImagePool_t {
    /* ----------------------- Add your image define here ----------------------- */
    struct AppStartupAnim_t {
        uint8_t logo_r[14280];
    };
    AppStartupAnim_t AppStartupAnim;
    struct AppUserDemo_t {
        uint8_t wifi_scan_img[7837];
        uint8_t i2c_scan_img[7750];
        uint8_t uart_mon_img[8222];
        uint8_t uart_mon_01_img[8785];
        uint8_t uart_mon_02_img[8840];
        uint8_t uart_mon_03_img[8755];
        uint8_t uart_mon_04_img[8827];
        uint8_t io_pwm_img[8479];
        uint8_t io_pwm_01_img[7727];
        uint8_t io_pwm_02_img[7932];
        uint8_t io_pwm_03_img[7902];
        uint8_t io_pwm_04_img[7925];
        uint8_t io_adc_img[8254];
        uint8_t io_adc_01_img[7526];
        uint8_t io_test_img[8067];
        uint8_t ir_send_img[8329];
        uint8_t ir_send_01_img[6008];
        uint8_t ir_send_02_img[6162];
        uint8_t imu_test_img[7929];

        uint8_t imu_cross_mark[8112];
        uint8_t imu_dial[47628];
        uint8_t imu_tilt_ball[5808];
    };
    AppUserDemo_t AppUserDemo;
};
