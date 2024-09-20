/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "../hal_atom_s3.h"
#include "../hal_config.h"
#include <mooncake.h>
extern "C" {
#include "../utils/ir_nec_transceiver/ir_nec_transceiver.h"
}

void HAL_AtomS3::ir_init()
{
    spdlog::info("ir init");

    ir_nec_transceiver_init(HAL_PIN_IR_TX);

    // /* -------------------------------------------------------------------------- */
    // /*                                    Test                                    */
    // /* -------------------------------------------------------------------------- */
    // uint16_t addr = 0x10;
    // uint16_t comm = 0x0;
    // while (1)
    // {
    //     feedTheDog();
    //     delay(1000);

    //     spdlog::info("ir send 0x{:04X} 0x{:04X}", addr, comm);
    //     ir_nec_transceiver_send(addr, comm);
    //     comm++;
    // }
}

void HAL_AtomS3::irSendNecMsg(uint16_t addr, uint16_t command)
{
    ir_nec_transceiver_send(addr, command);
}

bool HAL_AtomS3::irSendCmd(uint8_t cmd)
{
    irSendNecMsg(0x10, cmd);
    return true;
}
