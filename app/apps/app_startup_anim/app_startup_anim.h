/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <cstdint>
#include <mooncake.h>

namespace MOONCAKE {
namespace APPS {
/**
 * @brief AppStartupAnim
 *
 */
class AppStartupAnim : public APP_BASE {
public:
    void onResume() override;
    void onRunning() override;
    void onDestroy() override;

private:
    void startup_anim();
    void startup_anim_lvgl();
};

class AppStartupAnim_Packer : public APP_PACKER_BASE {
    const char* getAppName() override
    {
        return "StartupAnim";
    }
    void* newApp() override
    {
        return new AppStartupAnim;
    }
    void deleteApp(void* app) override
    {
        delete (AppStartupAnim*)app;
    }
};
}  // namespace APPS
}  // namespace MOONCAKE
