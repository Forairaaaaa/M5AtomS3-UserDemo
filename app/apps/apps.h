/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "app_template/app_template.h"
#include "app_startup_anim/app_startup_anim.h"
#include "app_user_demo/app_user_demo.h"
/* Header files locator (Don't remove) */

/**
 * @brief Run startup anim app
 *
 * @param mooncake
 */
inline void app_run_startup_anim(MOONCAKE::Mooncake* mooncake)
{
    /* -------------------- Install and run startup anim here ------------------- */
    auto startup_anim = new MOONCAKE::APPS::AppStartupAnim_Packer;
    mooncake->createAndStartApp(startup_anim);
    while (1) {
        mooncake->update();
        if (mooncake->getAppManager()->getCreatedAppNum() == 0) break;
    }
    delete startup_anim;
}

/**
 * @brief Install and start default startup app
 *
 * @param mooncake
 */
inline void app_install_default_startup_app(MOONCAKE::Mooncake* mooncake)
{
    // Launcher or daemons
    std::vector<MOONCAKE::APP_PACKER_BASE*> app_packers;

    app_packers.push_back(new MOONCAKE::APPS::AppUserDemo_Packer);

    for (const auto& i : app_packers) {
        mooncake->installApp(i);
        mooncake->createAndStartApp(i);
    }
}

/**
 * @brief Install apps
 *
 * @param mooncake
 */
inline void app_install_apps(MOONCAKE::Mooncake* mooncake)
{
    /* Install app locator (Don't remove) */
}
