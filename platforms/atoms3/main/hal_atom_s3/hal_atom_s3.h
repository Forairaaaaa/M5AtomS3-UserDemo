/**
 * @file hal_atom_s3.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2024-09-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <hal/hal.h>
#include <Arduino.h>

class HAL_AtomS3 : public HAL
{
private:
    void _watch_dog_init();
    void _disp_init();
    void _disp_test();
    void _i2c_init();
    void _gamepad_init();
    void _imu_init();
    void _imu_test();
    void _imu_keep_sending_data();
    void _ir_init();

public:
    std::string type() override { return "AtomS3"; }

    inline void init() override
    {
        _watch_dog_init();
        _i2c_init();
        _disp_init();
        _imu_init();
        _ir_init();

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
    void updateUartPassThrough(std::function<void(uint8_t)> onGrove2Usb, std::function<void(uint8_t)> onUsb2Grove) override;
    void stopUartPassThrough() override;

    void startPwm() override;
    void changePwmDuty(uint8_t duty) override;
    void stopPwm() override;

    int getAdcValue(uint8_t port) override;

    bool irSendCmd(uint8_t cmd) override;
};
