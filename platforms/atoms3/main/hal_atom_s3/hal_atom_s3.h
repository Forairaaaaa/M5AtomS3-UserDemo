/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <hal/hal.h>
#include <Arduino.h>

class HAL_AtomS3 : public HAL {
public:
    std::string type() override
    {
        return "AtomS3";
    }

    inline void init() override
    {
        watch_dog_init();
        i2c_init();
        disp_init();
        imu_init();
        ir_init();

        initArduino();
    }

    /* -------------------------------------------------------------------------- */
    /*                             Public api override                            */
    /* -------------------------------------------------------------------------- */
    void feedTheDog() override;
    bool getButton(GAMEPAD::GamePadButton_t button) override;
    void updateImuData() override;
    bool getImuInterruptState() override;
    bool isImuAvailable() override;
    bool isImuMagAvailable() override;
    void updateImuTiltBallOffset() override;
    void updateImuDialAngle() override;
    void irSendNecMsg(uint16_t addr, uint16_t command) override;
    bool lvglInit() override;
    void lvglTimerHandler() override;

    bool startWifiScan() override;
    bool isWifiScanFinish() override;
    std::vector<NETWORK::WifiScanResult_t>* getWifiScanResult() override;
    void freeWifiScanResult() override;

    bool startI2CScan() override;
    std::vector<uint8_t>* getI2cScanResult() override;
    void freeI2cScanResult() override;

    void startUartPassThrough(uint32_t baudrate, int pinRx, int pinTx) override;
    void updateUartPassThrough(std::function<void(uint8_t)> onGrove2Usb,
                               std::function<void(uint8_t)> onUsb2Grove) override;
    void stopUartPassThrough() override;

    void startPwm() override;
    void changePwmDuty(uint8_t duty) override;
    void stopPwm() override;

    int getAdcValue(uint8_t port) override;

    bool irSendCmd(uint8_t cmd) override;

private:
    void watch_dog_init();
    void disp_init();
    void disp_test();
    void i2c_init();
    void gamepad_init();
    void imu_init();
    void imu_test();
    void imu_keep_sending_data();
    void ir_init();
};
