/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef M5_UNIT_ENCODER_MOTOR_HPP
#define M5_UNIT_ENCODER_MOTOR_HPP

#include "M5UnitEncoderMotorTypes.hpp"
#include "M5Unit_4EncoderMotor_Driver.hpp"
#include "M5Unit_EncoderMotor_Driver.hpp"

class M5UnitEncoderMotor {
public:
    explicit M5UnitEncoderMotor(m5_unit_encoder_motor_driver_t driverType);

    M5UnitEncoderMotor(const M5UnitEncoderMotor&)            = delete;
    M5UnitEncoderMotor& operator=(const M5UnitEncoderMotor&) = delete;

    /**
     * @param address Set to 0 to use the selected product's default address.
     */
    bool begin(TwoWire* wire = &Wire, uint8_t address = 0, int8_t sda = 21, int8_t scl = 22,
               uint32_t frequency = 400000);
    bool isConnected();

    m5_unit_encoder_motor_driver_t driverType() const;
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

    /** Set the speed target and enter Speed mode without changing the PID. */
    bool runSpeed(int8_t speedPoint, uint8_t channel = 0);
    /** Set the speed PID and target, then enter Speed mode. */
    bool runSpeed(int8_t speedPoint, const M5UnitEncoderMotorPID& pid, uint8_t channel = 0);

    /** Set the position parameters and enter Position mode without changing the encoder value. */
    bool runPosition(int32_t positionPoint, int8_t maxSpeed, const M5UnitEncoderMotorPID& pid, uint8_t channel = 0);
    bool stop(uint8_t channel = 0);

    bool getMotorCurrent(float& currentAmps);
    bool getVinADC12(uint16_t& adcValue);
    bool getVinADC8(uint8_t& adcValue);
    bool getVinMillivolts(uint16_t& millivolts);
    bool getVinVoltage(float& volts);

    bool setEncoderMode(m5_unit_encoder_motor_encoder_mode_t mode);
    bool getEncoderMode(m5_unit_encoder_motor_encoder_mode_t& mode);
    bool setSoftStartStop(bool enabled, uint8_t channel = 0);
    bool getSoftStartStop(bool& enabled, uint8_t channel = 0);

    /** Motor sleep is supported only by Unit EncoderMotor. */
    bool setMotorRun(bool run);
    bool getMotorRun(bool& run);
    bool setMotorSleep(bool sleep);

    bool setRGBColor(uint8_t red, uint8_t green, uint8_t blue);
    bool getRGBColor(uint8_t& red, uint8_t& green, uint8_t& blue);
    bool setRGBBrightness(uint8_t brightness);
    bool getRGBBrightness(uint8_t& brightness);
    bool getUID(uint8_t uid[12]);
    bool setI2CAddress(uint8_t newAddress);
    bool getI2CAddress(uint8_t& address);

private:
    M5UnitEncoderMotorSingleDriver _singleDriver;
    M5Unit4EncoderMotorDriver _fourDriver;
    M5EncoderMotorDriverBase* _driver;
    m5_unit_encoder_motor_driver_t _driverType;
};

#endif
