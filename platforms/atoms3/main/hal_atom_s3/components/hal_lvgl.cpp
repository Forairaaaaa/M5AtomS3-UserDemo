/**
 * @file hal_display.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal_atom_s3.h"
#include "../hal_config.h"
#include <cstdlib>
#include <mooncake.h>
#include "spdlog/spdlog.h"
#include <lvgl.h>
// https://docs.lvgl.io/master/get-started/quick-overview.html#add-lvgl-into-your-project

static uint32_t _lvgl_tick_cb() { return HAL::Millis(); }

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
    void* buffer = NULL;
    buffer = malloc(128 * 128 * 2);
    assert(buffer != NULL);

    lv_display_t* display = lv_display_create(128, 128);
    lv_display_set_buffers(display, buffer, NULL, sizeof(uint16_t) * 128 * 128, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(display, _lvgl_disp_flush);

    return true;
}

void HAL_AtomS3::lvglTimerHandler() { lv_timer_handler(); }