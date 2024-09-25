/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <app.h>
#include "hal_desktop/hal_desktop.hpp"

void setup()
{
    APP::SetupCallback_t callback;

    callback.AssetPoolInjection = []() {
        // Create and output to bin
        AssetPool::CreateStaticAssetBin(AssetPool::CreateStaticAsset());
        // Load bin and inject
        AssetPool::InjectStaticAsset(AssetPool::GetStaticAssetFromBin());
    };

    callback.HalInjection = []() {
        // ATOMS3R Screen size is 128x128
        HAL::Inject(new HAL_Desktop(128, 128));
    };

    APP::Setup(callback);
}

void loop()
{
    APP::Loop();
}
