/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_atom_s3.h"
#include "../hal_config.h"
#include <mooncake.h>
#include "spdlog/spdlog.h"
#include <lvgl.h>
// https://docs.lvgl.io/master/get-started/quick-overview.html#add-lvgl-into-your-project

static uint32_t _lvgl_tick_cb()
{
    return HAL::Millis();
}

static void _lvgl_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
    int w = (area->x2 - area->x1 + 1);
    int h = (area->y2 - area->y1 + 1);

    HAL::GetDisplay()->startWrite();
    HAL::GetDisplay()->setAddrWindow(area->x1, area->y1, w, h);
    HAL::GetDisplay()->writePixels((lgfx::rgb565_t*)px_map, w * h);
    HAL::GetDisplay()->endWrite();

    lv_disp_flush_ready(disp);
}

bool HAL_AtomS3::lvglInit()
{
    spdlog::info("lvgl init");

    lv_init();

    // Tick
    lv_tick_set_cb(_lvgl_tick_cb);

    // Display
    lv_display_t* display = lv_display_create(_data.canvas->width(), _data.canvas->height());
    lv_display_set_buffers(display, _data.canvas->getBuffer(), NULL,
                           sizeof(uint16_t) * _data.canvas->width() * _data.canvas->height(),
                           LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(display, _lvgl_disp_flush);

    return true;
}

void HAL_AtomS3::lvglTimerHandler()
{
    lv_timer_handler();
}
