/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <lvgl.h>

lv_image_dsc_t ui_img_imu_cross_mark_png = {
    .header.w     = 52,
    .header.h     = 52,
    .header.cf    = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
};

lv_image_dsc_t ui_img_imu_dial_png = {
    .header.w     = 126,
    .header.h     = 126,
    .header.cf    = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
};

lv_image_dsc_t ui_img_imu_tilt_ball_png = {
    .header.w     = 44,
    .header.h     = 44,
    .header.cf    = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
};
