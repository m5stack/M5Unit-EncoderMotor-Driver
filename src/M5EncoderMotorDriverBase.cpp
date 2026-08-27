/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include "M5EncoderMotorDriverBase.hpp"

#include <string.h>

static_assert(sizeof(float) == 4, "M5UnitEncoderMotor requires a 32-bit float type");

M5EncoderMotorDriverBase::M5EncoderMotorDriverBase(uint8_t defaultAddress, uint8_t motorCount)
    : _wire(nullptr), _address(defaultAddress), _defaultAddress(defaultAddress), _motorCount(motorCount)
{
}

bool M5EncoderMotorDriverBase::begin(TwoWire* wire, uint8_t address, int8_t sda, int8_t scl, uint32_t frequency)
{
    if (wire == nullptr) {
        return false;
    }

    const uint8_t selectedAddress = address == 0 ? _defaultAddress : address;
    if (selectedAddress < M5_UNIT_ENCODER_MOTOR_MIN_ADDRESS || selectedAddress > M5_UNIT_ENCODER_MOTOR_MAX_ADDRESS) {
        return false;
    }

    _wire    = wire;
    _address = selectedAddress;
    _wire->begin(sda, scl, frequency);
    return isConnected();
}

bool M5EncoderMotorDriverBase::isConnected()
{
    if (_wire == nullptr) {
        return false;
    }
    _wire->beginTransmission(_address);
    return _wire->endTransmission() == 0;
}

uint8_t M5EncoderMotorDriverBase::motorCount() const
{
    return _motorCount;
}

uint8_t M5EncoderMotorDriverBase::getAddress() const
{
    return _address;
}

uint8_t M5EncoderMotorDriverBase::getDefaultAddress() const
{
    return _defaultAddress;
}

bool M5EncoderMotorDriverBase::isValidChannel(uint8_t channel) const
{
    return channel < _motorCount;
}

bool M5EncoderMotorDriverBase::readBytes(uint8_t reg, uint8_t* data, size_t length)
{
    if (_wire == nullptr || data == nullptr || length == 0 || length > 255) {
        return false;
    }

    _wire->beginTransmission(_address);
    if (_wire->write(reg) != 1 || _wire->endTransmission(false) != 0) {
        return false;
    }

    const size_t received = _wire->requestFrom(static_cast<int>(_address), static_cast<int>(length));
    if (received != length) {
        while (_wire->available()) {
            _wire->read();
        }
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        if (!_wire->available()) {
            return false;
        }
        data[i] = static_cast<uint8_t>(_wire->read());
    }
    return true;
}

bool M5EncoderMotorDriverBase::writeBytes(uint8_t reg, const uint8_t* data, size_t length)
{
    if (_wire == nullptr || data == nullptr || length == 0) {
        return false;
    }

    _wire->beginTransmission(_address);
    if (_wire->write(reg) != 1 || _wire->write(data, length) != length) {
        _wire->endTransmission();
        return false;
    }
    return _wire->endTransmission() == 0;
}

bool M5EncoderMotorDriverBase::readByte(uint8_t reg, uint8_t& data)
{
    return readBytes(reg, &data, 1);
}

bool M5EncoderMotorDriverBase::writeByte(uint8_t reg, uint8_t data)
{
    return writeBytes(reg, &data, 1);
}

bool M5EncoderMotorDriverBase::isValidMotorValue(int8_t value)
{
    return value >= -127;
}

uint32_t M5EncoderMotorDriverBase::decodeUInt32LE(const uint8_t data[4])
{
    return static_cast<uint32_t>(data[0]) | (static_cast<uint32_t>(data[1]) << 8) |
           (static_cast<uint32_t>(data[2]) << 16) | (static_cast<uint32_t>(data[3]) << 24);
}

void M5EncoderMotorDriverBase::encodeUInt32LE(uint32_t value, uint8_t data[4])
{
    data[0] = static_cast<uint8_t>(value);
    data[1] = static_cast<uint8_t>(value >> 8);
    data[2] = static_cast<uint8_t>(value >> 16);
    data[3] = static_cast<uint8_t>(value >> 24);
}

uint8_t M5EncoderMotorDriverBase::configRegister(uint8_t channel, uint8_t offset) const
{
    return static_cast<uint8_t>(REG_CONFIG + channel * 0x10 + offset);
}

bool M5EncoderMotorDriverBase::getFirmwareVersion(uint8_t& version)
{
    return readByte(REG_FIRMWARE_VERSION, version);
}

bool M5EncoderMotorDriverBase::setPWMDuty(int8_t duty, uint8_t channel)
{
    return isValidChannel(channel) && isValidMotorValue(duty) &&
           writeByte(static_cast<uint8_t>(REG_PWM_DUTY + channel), static_cast<uint8_t>(duty));
}

bool M5EncoderMotorDriverBase::getPWMDuty(int8_t& duty, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(static_cast<uint8_t>(REG_PWM_DUTY + channel), value)) {
        return false;
    }
    duty = static_cast<int8_t>(value);
    return true;
}

bool M5EncoderMotorDriverBase::setEncoder(int32_t encoder, uint8_t channel)
{
    if (!isValidChannel(channel)) {
        return false;
    }
    uint8_t data[4];
    encodeUInt32LE(static_cast<uint32_t>(encoder), data);
    return writeBytes(static_cast<uint8_t>(REG_ENCODER + channel * 4), data, sizeof(data));
}

bool M5EncoderMotorDriverBase::clearEncoder(uint8_t channel)
{
    return setEncoder(0, channel);
}

bool M5EncoderMotorDriverBase::getEncoder(int32_t& encoder, uint8_t channel)
{
    uint8_t data[4] = {};
    if (!isValidChannel(channel) || !readBytes(static_cast<uint8_t>(REG_ENCODER + channel * 4), data, sizeof(data))) {
        return false;
    }
    encoder = static_cast<int32_t>(decodeUInt32LE(data));
    return true;
}

bool M5EncoderMotorDriverBase::getSpeed(int8_t& speed, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(static_cast<uint8_t>(REG_SPEED + channel), value)) {
        return false;
    }
    speed = static_cast<int8_t>(value);
    return true;
}

bool M5EncoderMotorDriverBase::setConfig(const M5UnitEncoderMotorConfig& config, uint8_t channel)
{
    if (!isValidChannel(channel) || config.mode > M5_UNIT_ENCODER_MOTOR_MODE_SPEED ||
        !isValidMotorValue(config.positionMaxSpeed) || !isValidMotorValue(config.speedPoint)) {
        return false;
    }

    uint8_t data[13] = {
        static_cast<uint8_t>(config.mode),
        config.positionPID.kp,
        config.positionPID.ki,
        config.positionPID.kd,
        0,
        0,
        0,
        0,
        static_cast<uint8_t>(config.positionMaxSpeed),
        config.speedPID.kp,
        config.speedPID.ki,
        config.speedPID.kd,
        static_cast<uint8_t>(config.speedPoint),
    };
    encodeUInt32LE(static_cast<uint32_t>(config.positionPoint), &data[4]);
    return writeBytes(configRegister(channel, 0), data, sizeof(data));
}

bool M5EncoderMotorDriverBase::getConfig(M5UnitEncoderMotorConfig& config, uint8_t channel)
{
    uint8_t data[13] = {};
    if (!isValidChannel(channel) || !readBytes(configRegister(channel, 0), data, sizeof(data)) ||
        data[0] > M5_UNIT_ENCODER_MOTOR_MODE_SPEED) {
        return false;
    }

    config.mode             = static_cast<m5_unit_encoder_motor_mode_t>(data[0]);
    config.positionPID      = {data[1], data[2], data[3]};
    config.positionPoint    = static_cast<int32_t>(decodeUInt32LE(&data[4]));
    config.positionMaxSpeed = static_cast<int8_t>(data[8]);
    config.speedPID         = {data[9], data[10], data[11]};
    config.speedPoint       = static_cast<int8_t>(data[12]);
    return true;
}

bool M5EncoderMotorDriverBase::setMotorMode(m5_unit_encoder_motor_mode_t mode, uint8_t channel)
{
    return isValidChannel(channel) && mode <= M5_UNIT_ENCODER_MOTOR_MODE_SPEED &&
           writeByte(configRegister(channel, 0), static_cast<uint8_t>(mode));
}

bool M5EncoderMotorDriverBase::getMotorMode(m5_unit_encoder_motor_mode_t& mode, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(configRegister(channel, 0), value) ||
        value > M5_UNIT_ENCODER_MOTOR_MODE_SPEED) {
        return false;
    }
    mode = static_cast<m5_unit_encoder_motor_mode_t>(value);
    return true;
}

bool M5EncoderMotorDriverBase::setPositionPID(const M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    const uint8_t data[3] = {pid.kp, pid.ki, pid.kd};
    return isValidChannel(channel) && writeBytes(configRegister(channel, 1), data, sizeof(data));
}

bool M5EncoderMotorDriverBase::getPositionPID(M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    uint8_t data[3] = {};
    if (!isValidChannel(channel) || !readBytes(configRegister(channel, 1), data, sizeof(data))) {
        return false;
    }
    pid = {data[0], data[1], data[2]};
    return true;
}

bool M5EncoderMotorDriverBase::setPositionPoint(int32_t positionPoint, uint8_t channel)
{
    if (!isValidChannel(channel)) {
        return false;
    }
    uint8_t data[4];
    encodeUInt32LE(static_cast<uint32_t>(positionPoint), data);
    return writeBytes(configRegister(channel, 4), data, sizeof(data));
}

bool M5EncoderMotorDriverBase::getPositionPoint(int32_t& positionPoint, uint8_t channel)
{
    uint8_t data[4] = {};
    if (!isValidChannel(channel) || !readBytes(configRegister(channel, 4), data, sizeof(data))) {
        return false;
    }
    positionPoint = static_cast<int32_t>(decodeUInt32LE(data));
    return true;
}

bool M5EncoderMotorDriverBase::setPositionMaxSpeed(int8_t maxSpeed, uint8_t channel)
{
    return isValidChannel(channel) && isValidMotorValue(maxSpeed) &&
           writeByte(configRegister(channel, 8), static_cast<uint8_t>(maxSpeed));
}

bool M5EncoderMotorDriverBase::getPositionMaxSpeed(int8_t& maxSpeed, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(configRegister(channel, 8), value)) {
        return false;
    }
    maxSpeed = static_cast<int8_t>(value);
    return true;
}

bool M5EncoderMotorDriverBase::setSpeedPID(const M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    const uint8_t data[3] = {pid.kp, pid.ki, pid.kd};
    return isValidChannel(channel) && writeBytes(configRegister(channel, 9), data, sizeof(data));
}

bool M5EncoderMotorDriverBase::getSpeedPID(M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    uint8_t data[3] = {};
    if (!isValidChannel(channel) || !readBytes(configRegister(channel, 9), data, sizeof(data))) {
        return false;
    }
    pid = {data[0], data[1], data[2]};
    return true;
}

bool M5EncoderMotorDriverBase::setSpeedPoint(int8_t speedPoint, uint8_t channel)
{
    return isValidChannel(channel) && isValidMotorValue(speedPoint) &&
           writeByte(configRegister(channel, 12), static_cast<uint8_t>(speedPoint));
}

bool M5EncoderMotorDriverBase::getSpeedPoint(int8_t& speedPoint, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(configRegister(channel, 12), value)) {
        return false;
    }
    speedPoint = static_cast<int8_t>(value);
    return true;
}

bool M5EncoderMotorDriverBase::runSpeed(int8_t speedPoint, const M5UnitEncoderMotorPID& pid, uint8_t channel)
{
    if (!isValidChannel(channel) || !isValidMotorValue(speedPoint)) {
        return false;
    }

    const uint8_t data[4] = {pid.kp, pid.ki, pid.kd, static_cast<uint8_t>(speedPoint)};
    return writeBytes(configRegister(channel, 9), data, sizeof(data)) &&
           setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_SPEED, channel);
}

bool M5EncoderMotorDriverBase::runPosition(int32_t positionPoint, int8_t maxSpeed, const M5UnitEncoderMotorPID& pid,
                                           uint8_t channel)
{
    if (!isValidChannel(channel) || !isValidMotorValue(maxSpeed)) {
        return false;
    }

    uint8_t data[8] = {pid.kp, pid.ki, pid.kd, 0, 0, 0, 0, static_cast<uint8_t>(maxSpeed)};
    encodeUInt32LE(static_cast<uint32_t>(positionPoint), &data[3]);
    return writeBytes(configRegister(channel, 1), data, sizeof(data)) &&
           setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_POSITION, channel);
}

bool M5EncoderMotorDriverBase::stop(uint8_t channel)
{
    return setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_NORMAL, channel) && setPWMDuty(0, channel);
}

bool M5EncoderMotorDriverBase::getMotorCurrent(float& currentAmps)
{
    uint8_t data[4] = {};
    if (!readBytes(REG_MOTOR_CURRENT, data, sizeof(data))) {
        return false;
    }
    const uint32_t bits = decodeUInt32LE(data);
    memcpy(&currentAmps, &bits, sizeof(currentAmps));
    return true;
}

bool M5EncoderMotorDriverBase::getVinADC12(uint16_t& adcValue)
{
    uint8_t data[2] = {};
    if (!readBytes(REG_VIN_ADC12, data, sizeof(data))) {
        return false;
    }
    adcValue = static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1] & 0x0F) << 8);
    return true;
}

bool M5EncoderMotorDriverBase::getVinMillivolts(uint16_t& millivolts)
{
    uint8_t data[2] = {};
    if (!readBytes(REG_VIN_MILLIVOLTS, data, sizeof(data))) {
        return false;
    }
    millivolts = static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8);
    return true;
}

bool M5EncoderMotorDriverBase::getVinVoltage(float& volts)
{
    uint16_t millivolts = 0;
    if (!getVinMillivolts(millivolts)) {
        return false;
    }
    volts = millivolts / 1000.0f;
    return true;
}

bool M5EncoderMotorDriverBase::setEncoderMode(m5_unit_encoder_motor_encoder_mode_t mode)
{
    return mode <= M5_UNIT_ENCODER_MOTOR_ENCODER_BA && writeByte(REG_ENCODER_MODE, static_cast<uint8_t>(mode));
}

bool M5EncoderMotorDriverBase::getEncoderMode(m5_unit_encoder_motor_encoder_mode_t& mode)
{
    uint8_t value = 0;
    if (!readByte(REG_ENCODER_MODE, value) || value > M5_UNIT_ENCODER_MOTOR_ENCODER_BA) {
        return false;
    }
    mode = static_cast<m5_unit_encoder_motor_encoder_mode_t>(value);
    return true;
}

bool M5EncoderMotorDriverBase::setMotorRun(bool)
{
    return false;
}

bool M5EncoderMotorDriverBase::getMotorRun(bool&)
{
    return false;
}

bool M5EncoderMotorDriverBase::setRGBColor(uint8_t red, uint8_t green, uint8_t blue)
{
    const uint8_t data[4] = {blue, green, red, 0};
    return writeBytes(REG_RGB, data, sizeof(data));
}

bool M5EncoderMotorDriverBase::getRGBColor(uint8_t& red, uint8_t& green, uint8_t& blue)
{
    uint8_t data[3] = {};
    if (!readBytes(REG_RGB, data, sizeof(data))) {
        return false;
    }
    blue  = data[0];
    green = data[1];
    red   = data[2];
    return true;
}

bool M5EncoderMotorDriverBase::setRGBBrightness(uint8_t brightness)
{
    return writeByte(REG_RGB_BRIGHTNESS, brightness);
}

bool M5EncoderMotorDriverBase::getRGBBrightness(uint8_t& brightness)
{
    return readByte(REG_RGB_BRIGHTNESS, brightness);
}

bool M5EncoderMotorDriverBase::getUID(uint8_t uid[12])
{
    return readBytes(REG_UID, uid, 12);
}

bool M5EncoderMotorDriverBase::setI2CAddress(uint8_t newAddress)
{
    if (newAddress < M5_UNIT_ENCODER_MOTOR_MIN_ADDRESS || newAddress > M5_UNIT_ENCODER_MOTOR_MAX_ADDRESS ||
        !writeByte(REG_I2C_ADDRESS, newAddress)) {
        return false;
    }

    _address = newAddress;
    return true;
}

bool M5EncoderMotorDriverBase::getI2CAddress(uint8_t& address)
{
    return readByte(REG_I2C_ADDRESS, address);
}
