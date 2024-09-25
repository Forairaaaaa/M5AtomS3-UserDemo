/**
 * @file ir_nec_transceiver.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2024-07-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <stdint.h>

void ir_nec_transceiver_init(int txPin);
void ir_nec_transceiver_send(uint16_t addr, uint16_t cmmd);
