/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "hal/utils/lgfx_fx/lgfx_fx.h"
#include <cstdint>
#include <mooncake.h>
#include <vector>
#include "view/view.h"
// https://github.com/m5stack/M5AtomS3-UserDemo/blob/main/src/main.cpp

namespace MOONCAKE {
namespace APPS {
/**
 * @brief AppUserDemo
 *
 */
class AppUserDemo : public APP_BASE {
public:
    void onResume() override;
    void onRunning() override;
    void onDestroy() override;

private:
    struct Data_t {
        std::vector<uint8_t*> func_img_list;
        std::vector<VIEW::func_base_t*> func_list;
        VIEW::func_index_t func_index = VIEW::FUNC_WIFI_SCAN;
        bool is_entry_func            = false;
    };
    Data_t _data;
};

class AppUserDemo_Packer : public APP_PACKER_BASE {
    const char* getAppName() override
    {
        return "AppUserDemo";
    };
    void* newApp() override
    {
        return new AppUserDemo;
    }
    void deleteApp(void* app) override
    {
        delete (AppUserDemo*)app;
    }
};
}  // namespace APPS
}  // namespace MOONCAKE
