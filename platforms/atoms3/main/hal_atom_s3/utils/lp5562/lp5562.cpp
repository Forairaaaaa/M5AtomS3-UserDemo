/**
 * @file lp5562.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-07-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "lp5562.h"

#define LP5562_I2C_ADDRESS 0x30

#define LP5562_MAX_LEDS 4

#define LP5562_REG_ENABLE 0x00
#define LP5562_EXEC_ENG1_M 0x30
#define LP5562_EXEC_ENG2_M 0x0C
#define LP5562_EXEC_ENG3_M 0x03
#define LP5562_EXEC_M 0x3F
#define LP5562_MASTER_ENABLE 0x40
#define LP5562_LOGARITHMIC_PWM 0x80
#define LP5562_EXEC_RUN 0x2A
#define LP5562_ENABLE_DEFAULT (LP5562_MASTER_ENABLE | LP5562_LOGARITHMIC_PWM)
#define LP5562_ENABLE_RUN_PROGRAM (LP5562_ENABLE_DEFAULT | LP5562_EXEC_RUN)

#define LP5562_REG_OP_MODE 0x01

#define LP5562_REG_R_PWM 0x04
#define LP5562_REG_G_PWM 0x03
#define LP5562_REG_B_PWM 0x02
#define LP5562_REG_W_PWM 0x0E

#define LP5562_REG_R_CURRENT 0x07
#define LP5562_REG_G_CURRENT 0x06
#define LP5562_REG_B_CURRENT 0x05
#define LP5562_REG_W_CURRENT 0x0F

#define LP5562_REG_CONFIG 0x08
#define LP5562_PWM_HF 0x40
#define LP5562_PWRSAVE_EN 0x20
#define LP5562_CLK_INT 0x01
#define LP5562_DEFAULT_CFG (LP5562_PWM_HF | LP5562_PWRSAVE_EN)

#define LP5562_REG_RESET 0x0D
#define LP5562_RESET 0xFF

#define LP5562_REG_PROG_MEM_ENG1 0x10
#define LP5562_REG_PROG_MEM_ENG2 0x30
#define LP5562_REG_PROG_MEM_ENG3 0x50

#define LP5562_REG_ENG_SEL 0x70
#define LP5562_ENG_SEL_PWM 0
#define LP5562_ENG_FOR_RGB_M 0x3F
#define LP5562_ENG_SEL_RGB 0x1B
#define LP5562_ENG_FOR_W_M 0xC0
#define LP5562_ENG1_FOR_W 0x40
#define LP5562_ENG2_FOR_W 0x80
#define LP5562_ENG3_FOR_W 0xC0

#define LP5562_CMD_DISABLE 0x00
#define LP5562_CMD_LOAD 0x15
#define LP5562_CMD_RUN 0x2A
#define LP5562_CMD_DIRECT 0x3F
#define LP5562_PATTERN_OFF 0

bool LP5562_Class::begin()
{
    _delay(1);

    // Set chip_en to 1
    if (!_write_register(0x00, 0B01000000))
        return false;
    _delay(1);

    // Enable internal clock
    _write_register(0x08, 0B00000001);

    // Configure all LED outputs to be controlled from I2C registers
    _write_register(0x70, 0B00000000);

    return true;
}

void LP5562_Class::setLedCurrent(uint8_t channel, uint8_t current)
{
    static const uint8_t addr[] = {
        LP5562_REG_R_CURRENT,
        LP5562_REG_G_CURRENT,
        LP5562_REG_B_CURRENT,
        LP5562_REG_W_CURRENT,
    };

    if (channel < LP5562_MAX_LEDS)
    {
        _write_register(addr[channel], current);
    }
}

void LP5562_Class::setBrightness(uint8_t channel, uint8_t brightness)
{
    static const uint8_t addr[] = {
        LP5562_REG_R_PWM,
        LP5562_REG_G_PWM,
        LP5562_REG_B_PWM,
        LP5562_REG_W_PWM,
    };

    if (channel < LP5562_MAX_LEDS)
    {
        _write_register(addr[channel], brightness);
    }
}

void LP5562_Class::reset() { _write_register(LP5562_REG_RESET, LP5562_RESET); }

void LP5562_Class::enable() { _write_register(LP5562_REG_ENABLE, LP5562_ENABLE_DEFAULT); }

void LP5562_Class::disable() { _write_register(LP5562_REG_ENABLE, 0x00); }

void LP5562_Class::runProgram() { _write_register(LP5562_REG_ENABLE, LP5562_ENABLE_RUN_PROGRAM); }

void LP5562_Class::stopProgram() { _write_register(LP5562_REG_ENABLE, LP5562_ENABLE_DEFAULT); }

/* -------------------------------------------------------------------------- */
/*                                    Port                                    */
/* -------------------------------------------------------------------------- */
bool LP5562_Class::_write_register(uint8_t reg, uint8_t value) { return writeRegister8(reg, value); }

uint8_t LP5562_Class::_read_register(uint8_t reg) { return readRegister8(reg); }

void LP5562_Class::_delay(uint32_t ms) { m5gfx::delay(ms); }
