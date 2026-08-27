/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef M5_ENCODER_MOTOR_DRIVER_BASE_HPP
#define M5_ENCODER_MOTOR_DRIVER_BASE_HPP

#include <Wire.h>

#include "M5UnitEncoderMotorTypes.hpp"

class M5EncoderMotorDriverBase {
public:
    virtual ~M5EncoderMotorDriverBase() = default;

    bool begin(TwoWire* wire, uint8_t address, int8_t sda, int8_t scl, uint32_t frequency);
    bool isConnected();

    uint8_t motorCount() const;
    uint8_t getAddress() const;
    uint8_t getDefaultAddress() const;

    bool getFirmwareVersion(uint8_t& version);

    bool setPWMDuty(int8_t duty, uint8_t channel = 0);
    bool getPWMDuty(int8_t& duty, uint8_t channel = 0);
    bool setEncoder(int32_t encoder, uint8_t channel = 0);
    bool clearEncoder(uint8_t channel = 0);
    bool getEncoder(int32_t& encoder, uint8_t channel = 0);
    bool getSpeed(int8_t& speed, uint8_t channel = 0);

    bool setConfig(const M5UnitEncoderMotorConfig& config, uint8_t channel = 0);
    bool getConfig(M5UnitEncoderMotorConfig& config, uint8_t channel = 0);
    bool setMotorMode(m5_unit_encoder_motor_mode_t mode, uint8_t channel = 0);
    bool getMotorMode(m5_unit_encoder_motor_mode_t& mode, uint8_t channel = 0);
    bool setPositionPID(const M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool getPositionPID(M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool setPositionPoint(int32_t positionPoint, uint8_t channel = 0);
    bool getPositionPoint(int32_t& positionPoint, uint8_t channel = 0);
    bool setPositionMaxSpeed(int8_t maxSpeed, uint8_t channel = 0);
    bool getPositionMaxSpeed(int8_t& maxSpeed, uint8_t channel = 0);
    bool setSpeedPID(const M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool getSpeedPID(M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool setSpeedPoint(int8_t speedPoint, uint8_t channel = 0);
    bool getSpeedPoint(int8_t& speedPoint, uint8_t channel = 0);

    bool runSpeed(int8_t speedPoint, const M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool runPosition(int32_t positionPoint, int8_t maxSpeed, const M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool stop(uint8_t channel = 0);

    bool getMotorCurrent(float& currentAmps);
    bool getVinADC12(uint16_t& adcValue);
    bool getVinMillivolts(uint16_t& millivolts);
    bool getVinVoltage(float& volts);

    bool setEncoderMode(m5_unit_encoder_motor_encoder_mode_t mode);
    bool getEncoderMode(m5_unit_encoder_motor_encoder_mode_t& mode);
    virtual bool setSoftStartStop(bool enabled, uint8_t channel = 0)  = 0;
    virtual bool getSoftStartStop(bool& enabled, uint8_t channel = 0) = 0;
    virtual bool setMotorRun(bool run);
    virtual bool getMotorRun(bool& run);

    bool setRGBColor(uint8_t red, uint8_t green, uint8_t blue);
    bool getRGBColor(uint8_t& red, uint8_t& green, uint8_t& blue);
    bool setRGBBrightness(uint8_t brightness);
    bool getRGBBrightness(uint8_t& brightness);
    bool getUID(uint8_t uid[12]);
    bool setI2CAddress(uint8_t newAddress);
    bool getI2CAddress(uint8_t& address);

protected:
    M5EncoderMotorDriverBase(uint8_t defaultAddress, uint8_t motorCount);

    enum Register : uint8_t {
        REG_PWM_DUTY         = 0x20,
        REG_ENCODER          = 0x30,
        REG_SPEED            = 0x40,
        REG_CONFIG           = 0x50,
        REG_MOTOR_CURRENT    = 0x90,
        REG_VIN_ADC12        = 0xB0,
        REG_VIN_MILLIVOLTS   = 0xB2,
        REG_RGB              = 0xC0,
        REG_RGB_BRIGHTNESS   = 0xC4,
        REG_ENCODER_MODE     = 0xD0,
        REG_SOFT_START_STOP  = 0xD1,
        REG_MOTOR_SLEEP      = 0xD2,
        REG_UID              = 0xE0,
        REG_FIRMWARE_VERSION = 0xFE,
        REG_I2C_ADDRESS      = 0xFF,
    };

    bool isValidChannel(uint8_t channel) const;
    bool readBytes(uint8_t reg, uint8_t* data, size_t length);
    bool writeBytes(uint8_t reg, const uint8_t* data, size_t length);
    bool readByte(uint8_t reg, uint8_t& data);
    bool writeByte(uint8_t reg, uint8_t data);

private:
    uint8_t configRegister(uint8_t channel, uint8_t offset) const;
    static bool isValidMotorValue(int8_t value);
    static uint32_t decodeUInt32LE(const uint8_t data[4]);
    static void encodeUInt32LE(uint32_t value, uint8_t data[4]);

    TwoWire* _wire;
    uint8_t _address;
    const uint8_t _defaultAddress;
    const uint8_t _motorCount;
};

#endif
