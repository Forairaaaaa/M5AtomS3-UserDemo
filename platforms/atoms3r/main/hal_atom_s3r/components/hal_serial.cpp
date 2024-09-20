/**
 * @file hal_serial.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-08-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal_atom_s3r.h"
#include "../hal_config.h"
#include <mooncake.h>
#include <Arduino.h>

static USBCDC* _usb_serial = nullptr;

void HAL_AtomS3R::startUartPassThrough(uint32_t baudrate, int pinRx, int pinTx)
{
    spdlog::info("start serial pass through in {} {} {}", baudrate, pinRx, pinTx);

    Serial1.end();
    Serial1.begin(baudrate, SERIAL_8N1, pinRx, pinTx);

    if (_usb_serial == nullptr)
    {
        _usb_serial = new USBCDC(0);
        _usb_serial->begin(115200);
    }
}

void HAL_AtomS3R::updateUartPassThrough(std::function<void(uint8_t)> onGrove2Usb, std::function<void(uint8_t)> onUsb2Grove)
{
    /* ------------------------------- Grove 2 usb ------------------------------ */
    while (Serial1.available())
    {
        uint8_t c = Serial1.read();
        _usb_serial->write(c);
        onGrove2Usb(c);
    }

    /* ------------------------------- Usb 2 grove ------------------------------ */
    while (_usb_serial->available())
    {
        uint8_t c = _usb_serial->read();
        Serial1.write(c);
        onUsb2Grove(c);
    }
}

void HAL_AtomS3R::stopUartPassThrough()
{
    spdlog::info("stop serial pass through");

    Serial1.end();

    if (_usb_serial != nullptr)
    {
        _usb_serial->end();
        delete _usb_serial;
        _usb_serial = nullptr;
    }
}

/* -------------------------------------------------------------------------- */
/*                                     PWM                                    */
/* -------------------------------------------------------------------------- */
void HAL_AtomS3R::startPwm()
{
    ledcAttach(1, 1000, 8);
    ledcAttach(2, 1000, 8);
}

void HAL_AtomS3R::changePwmDuty(uint8_t duty)
{
    ledcWrite(1, duty);
    ledcWrite(2, duty);
}

void HAL_AtomS3R::stopPwm()
{
    ledcDetach(1);
    ledcDetach(2);
    gpio_reset_pin((gpio_num_t)1);
    gpio_reset_pin((gpio_num_t)2);
}

/* -------------------------------------------------------------------------- */
/*                                     ADC                                    */
/* -------------------------------------------------------------------------- */
int HAL_AtomS3R::getAdcValue(uint8_t port)
{
    if (port == 1)
        return analogRead(1);
    return analogRead(2);
}