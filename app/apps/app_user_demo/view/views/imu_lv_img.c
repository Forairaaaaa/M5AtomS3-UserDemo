/**
 * @file imu_lv_img.c
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-08-08
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <lvgl.h>

lv_image_dsc_t ui_img_imu_cross_mark_png = {
    .header.w = 52,
    .header.h = 52,
    .header.cf = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
};

lv_image_dsc_t ui_img_imu_dial_png = {
    .header.w = 126,
    .header.h = 126,
    .header.cf = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
};

lv_image_dsc_t ui_img_imu_tilt_ball_png = {
    .header.w = 44,
    .header.h = 44,
    .header.cf = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
};
