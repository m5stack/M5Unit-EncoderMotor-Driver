/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include "M5UnitEncoderMotor.hpp"

M5UnitEncoderMotor::M5UnitEncoderMotor(m5_unit_encoder_motor_driver_t driverType)
    : _driver(driverType == M5_UNIT_4ENCODER_MOTOR_DRIVER ? static_cast<M5EncoderMotorDriverBase*>(&_fourDriver)
                                                          : static_cast<M5EncoderMotorDriverBase*>(&_singleDriver)),
      _driverType(driverType)
{
}

bool M5UnitEncoderMotor::begin(TwoWire* wire, uint8_t address, int8_t sda, int8_t scl, uint32_t frequency)
{
    if (_driverType != M5_UNIT_ENCODER_MOTOR_DRIVER && _driverType != M5_UNIT_4ENCODER_MOTOR_DRIVER) {
        return false;
    }
    return _driver->begin(wire, address, sda, scl, frequency);
}

bool M5UnitEncoderMotor::isConnected()
{
    return _driver->isConnected();
}

m5_unit_encoder_motor_driver_t M5UnitEncoderMotor::driverType() const
{
    return _driverType;
}

uint8_t M5UnitEncoderMotor::motorCount() const
{
    return _driver->motorCount();
}

uint8_t M5UnitEncoderMotor::getAddress() const
{
    return _driver->getAddress();
}

uint8_t M5UnitEncoderMotor::getDefaultAddress() const
{
    return _driver->getDefaultAddress();
}

bool M5UnitEncoderMotor::getFirmwareVersion(uint8_t& version)
{
    return _driver->getFirmwareVersion(version);
}

bool M5UnitEncoderMotor::setPWMDuty(int8_t duty, uint8_t channel)
{
    return _driver->setPWMDuty(duty, channel);
}

bool M5UnitEncoderMotor::getPWMDuty(int8_t& duty, uint8_t channel)
{
    return _driver->getPWMDuty(duty, channel);
}

bool M5UnitEncoderMotor::setEncoder(int32_t encoder, uint8_t channel)
{
    return _driver->setEncoder(encoder, channel);
}

bool M5UnitEncoderMotor::clearEncoder(uint8_t channel)
{
    return _driver->clearEncoder(channel);
}

bool M5UnitEncoderMotor::getEncoder(int32_t& encoder, uint8_t channel)
{
    return _driver->getEncoder(encoder, channel);
}

bool M5UnitEncoderMotor::getSpeed(int8_t& speed, uint8_t channel)
{
    return _driver->getSpeed(speed, channel);
}

bool M5UnitEncoderMotor::setConfig(const M5UnitEncoderMotorConfig& config, uint8_t channel)
{
    return _driver->setConfig(config, channel);
}

bool M5UnitEncoderMotor::getConfig(M5UnitEncoderMotorConfig& config, uint8_t channel)
{
    return _driver->getConfig(config, channel);
}

bool M5UnitEncoderMotor::setMotorMode(m5_unit_encoder_motor_mode_t mode, uint8_t channel)
{
    return _driver->setMotorMode(mode, channel);
}

bool M5UnitEncoderMotor::getMotorMode(m5_unit_encoder_motor_mode_t& mode, uint8_t channel)
{
    return _driver->getMotorMode(mode, channel);
}

bool M5UnitEncoderMotor::setPositionPID(const M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    return _driver->setPositionPID(pid, channel);
}

bool M5UnitEncoderMotor::getPositionPID(M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    return _driver->getPositionPID(pid, channel);
}

bool M5UnitEncoderMotor::setPositionPoint(int32_t positionPoint, uint8_t channel)
{
    return _driver->setPositionPoint(positionPoint, channel);
}

bool M5UnitEncoderMotor::getPositionPoint(int32_t& positionPoint, uint8_t channel)
{
    return _driver->getPositionPoint(positionPoint, channel);
}

bool M5UnitEncoderMotor::setPositionMaxSpeed(int8_t maxSpeed, uint8_t channel)
{
    return _driver->setPositionMaxSpeed(maxSpeed, channel);
}

bool M5UnitEncoderMotor::getPositionMaxSpeed(int8_t& maxSpeed, uint8_t channel)
{
    return _driver->getPositionMaxSpeed(maxSpeed, channel);
}

bool M5UnitEncoderMotor::setSpeedPID(const M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    return _driver->setSpeedPID(pid, channel);
}

bool M5UnitEncoderMotor::getSpeedPID(M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    return _driver->getSpeedPID(pid, channel);
}

bool M5UnitEncoderMotor::setSpeedPoint(int8_t speedPoint, uint8_t channel)
{
    return _driver->setSpeedPoint(speedPoint, channel);
}

bool M5UnitEncoderMotor::getSpeedPoint(int8_t& speedPoint, uint8_t channel)
{
    return _driver->getSpeedPoint(speedPoint, channel);
}

bool M5UnitEncoderMotor::runSpeed(int8_t speedPoint, uint8_t channel)
{
    return _driver->setSpeedPoint(speedPoint, channel) &&
           _driver->setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_SPEED, channel);
}

bool M5UnitEncoderMotor::runSpeed(int8_t speedPoint, const M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    return _driver->runSpeed(speedPoint, pid, channel);
}

bool M5UnitEncoderMotor::runPosition(int32_t positionPoint, int8_t maxSpeed, const M5UnitEncoderMotorPID& pid,
                                     uint8_t channel)
{
    return _driver->runPosition(positionPoint, maxSpeed, pid, channel);
}

bool M5UnitEncoderMotor::stop(uint8_t channel)
{
    return _driver->stop(channel);
}

bool M5UnitEncoderMotor::getMotorCurrent(float& currentAmps)
{
    return _driver->getMotorCurrent(currentAmps);
}

bool M5UnitEncoderMotor::getVinADC12(uint16_t& adcValue)
{
    return _driver->getVinADC12(adcValue);
}

bool M5UnitEncoderMotor::getVinADC8(uint8_t& adcValue)
{
    uint16_t adc12 = 0;
    if (!getVinADC12(adc12)) {
        return false;
    }
    adcValue = static_cast<uint8_t>(adc12 >> 4);
    return true;
}

bool M5UnitEncoderMotor::getVinMillivolts(uint16_t& millivolts)
{
    return _driver->getVinMillivolts(millivolts);
}

bool M5UnitEncoderMotor::getVinVoltage(float& volts)
{
    return _driver->getVinVoltage(volts);
}

bool M5UnitEncoderMotor::setEncoderMode(m5_unit_encoder_motor_encoder_mode_t mode)
{
    return _driver->setEncoderMode(mode);
}

bool M5UnitEncoderMotor::getEncoderMode(m5_unit_encoder_motor_encoder_mode_t& mode)
{
    return _driver->getEncoderMode(mode);
}

bool M5UnitEncoderMotor::setSoftStartStop(bool enabled, uint8_t channel)
{
    return _driver->setSoftStartStop(enabled, channel);
}

bool M5UnitEncoderMotor::getSoftStartStop(bool& enabled, uint8_t channel)
{
    return _driver->getSoftStartStop(enabled, channel);
}

bool M5UnitEncoderMotor::setMotorRun(bool run)
{
    return _driver->setMotorRun(run);
}

bool M5UnitEncoderMotor::getMotorRun(bool& run)
{
    return _driver->getMotorRun(run);
}

bool M5UnitEncoderMotor::setMotorSleep(bool sleep)
{
    return setMotorRun(!sleep);
}

bool M5UnitEncoderMotor::setRGBColor(uint8_t red, uint8_t green, uint8_t blue)
{
    return _driver->setRGBColor(red, green, blue);
}

bool M5UnitEncoderMotor::getRGBColor(uint8_t& red, uint8_t& green, uint8_t& blue)
{
    return _driver->getRGBColor(red, green, blue);
}

bool M5UnitEncoderMotor::setRGBBrightness(uint8_t brightness)
{
    return _driver->setRGBBrightness(brightness);
}

bool M5UnitEncoderMotor::getRGBBrightness(uint8_t& brightness)
{
    return _driver->getRGBBrightness(brightness);
}

bool M5UnitEncoderMotor::getUID(uint8_t uid[12])
{
    return _driver->getUID(uid);
}

bool M5UnitEncoderMotor::setI2CAddress(uint8_t newAddress)
{
    return _driver->setI2CAddress(newAddress);
}

bool M5UnitEncoderMotor::getI2CAddress(uint8_t& address)
{
    return _driver->getI2CAddress(address);
}
