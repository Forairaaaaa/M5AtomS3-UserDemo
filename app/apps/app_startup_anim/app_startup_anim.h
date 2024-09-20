/**
 * @file app_startup_anim.h
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cstdint>
#include <mooncake.h>

namespace MOONCAKE
{
    namespace APPS
    {
        /**
         * @brief AppStartupAnim
         *
         */
        class AppStartupAnim : public APP_BASE
        {
        private:
            void _startup_anim();
            void _startup_anim_lvgl();

        public:
            void onResume() override;
            void onRunning() override;
            void onDestroy() override;
        };

        class AppStartupAnim_Packer : public APP_PACKER_BASE
        {
            const char* getAppName() override { return "StartupAnim"; }
            void* newApp() override { return new AppStartupAnim; }
            void deleteApp(void* app) override { delete (AppStartupAnim*)app; }
        };
    } // namespace APPS
} // namespace MOONCAKE
