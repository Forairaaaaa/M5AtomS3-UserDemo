/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "app_startup_anim.h"
#include "../../hal/hal.h"
#include "../../assets/assets.h"
#include "../utils/system/system.h"
#include "core/easing_path/easing_path.h"
#include "core/transition2d/transition2d.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <anim/lv_example_anim.h>
#include <benchmark/lv_demo_benchmark.h>
#include <cstdint>
#include <demos/stress/lv_demo_stress.h>
#include <event/lv_example_event.h>
#include <examples/widgets/lv_example_widgets.h>
#include <smooth_ui_toolkit.h>
#include <src/core/lv_obj_tree.h>
#include <src/display/lv_display.h>
#include <vector>
#include <lvgl.h>
#include <examples/lv_examples.h>
// https://github.com/m5stack/M5AtomS3-UserDemo/blob/main/src/main.cpp

using namespace MOONCAKE::APPS;
using namespace SYSTEM::INPUTS;
using namespace SYSTEM::UI;
using namespace SmoothUIToolKit;

void AppStartupAnim::onResume()
{
    spdlog::info("{} onResume", getAppName());
    HAL::GetDisplay()->setBrightness(255);
}

void AppStartupAnim::onRunning()
{
    // _startup_anim();
    startup_anim_lvgl();
    destroyApp();
}

void AppStartupAnim::onDestroy()
{
    spdlog::info("{} onDestroy", getAppName());
}

void AppStartupAnim::startup_anim()
{
    uint32_t circle_color_list[8] = {0xcc3300, 0xff6633, 0xffff66, 0x33cc33, 0x00ffff, 0x0000ff, 0xff3399, 0x990099};

    std::vector<Transition2D> anim_list;
    anim_list.resize(8);
    for (int i = 0; i < anim_list.size(); i++) {
        anim_list[i].setTransitionPath(EasingPath::easeOutBack);
        anim_list[i].setDuration(400);
        anim_list[i].setDelay(i * 60);
        anim_list[i].jumpTo(0 - (i + 1) * 23, HAL::GetCanvas()->height() + (i + 1) * 23);
        anim_list[i].moveTo(0, HAL::GetCanvas()->height());
    }

    while (1) {
        HAL::GetCanvas()->fillScreen(TFT_BLACK);

        for (int i = anim_list.size() - 1; i > 0; i--) {
            anim_list[i].update(HAL::Millis());
            auto frame = anim_list[i].getValue();
            HAL::GetCanvas()->fillSmoothCircle(frame.x, frame.y, (i + 1) * 23, circle_color_list[i]);
        }

        if (anim_list[anim_list.size() - 1].isFinish()) break;

        HAL::CanvasUpdate();
        HAL::FeedTheDog();
    }

    HAL::Delay(500);
}

extern lv_image_dsc_t ui_img_logo_r_png;

void AppStartupAnim::startup_anim_lvgl()
{
    uint32_t circle_color_list[8] = {0xcc3300, 0xff6633, 0xffff66, 0x33cc33, 0x00ffff, 0x0000ff, 0xff3399, 0x990099};

    std::vector<Transition2D> anim_list;
    anim_list.resize(8);
    for (int i = anim_list.size() - 1; i > 0; i--) {
        anim_list[i].setTransitionPath(EasingPath::easeOutBack);
        anim_list[i].setDuration(600);
        anim_list[i].setDelay(i * 40);
        anim_list[i].jumpTo(0 - (i + 1) * 23, LV_VER_RES + (i + 1) * 23);
        anim_list[i].moveTo(0, LV_VER_RES);
    }

    // Set screen background to black and remove scrollbars
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);
    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);

    // Circles
    std::vector<lv_obj_t*> circles;
    circles.resize(anim_list.size());
    for (int i = circles.size() - 1; i > 0; i--) {
        // Create circles once
        circles[i] = lv_obj_create(lv_screen_active());
        lv_obj_set_size(circles[i], (i + 1) * 46, (i + 1) * 46);  // Diameter is radius * 2
        lv_obj_set_style_radius(circles[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(circles[i], lv_color_hex(circle_color_list[i]), 0);
        lv_obj_set_style_border_width(circles[i], 0, 0);  // Remove border
    }

    // Logo R
    if (HAL::Type() == "AtomS3R") {
        ui_img_logo_r_png.data      = AssetPool::GetImage().AppStartupAnim.logo_r;
        ui_img_logo_r_png.data_size = sizeof(AssetPool::GetImage().AppStartupAnim.logo_r);

        lv_obj_t* img_logo_r = lv_image_create(lv_screen_active());
        lv_image_set_src(img_logo_r, &ui_img_logo_r_png);
        lv_obj_align(img_logo_r, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_width(img_logo_r, LV_SIZE_CONTENT);
        lv_obj_set_height(img_logo_r, LV_SIZE_CONTENT);
        lv_obj_add_flag(img_logo_r, LV_OBJ_FLAG_ADV_HITTEST);
        lv_obj_remove_flag(img_logo_r, LV_OBJ_FLAG_SCROLLABLE);
    }

    // Update and render
    while (1) {
        for (int i = anim_list.size() - 1; i > 0; i--) {
            anim_list[i].update(lv_tick_get());
            auto frame = anim_list[i].getValue();
            lv_obj_align(circles[i], LV_ALIGN_CENTER, frame.x - LV_HOR_RES / 2, frame.y - LV_VER_RES / 2);
        }

        if (anim_list[anim_list.size() - 1].isFinish()) break;

        HAL::LvglTimerHandler();
        HAL::FeedTheDog();
    }

    lv_obj_clean(lv_screen_active());

    HAL::Delay(1000);
}
