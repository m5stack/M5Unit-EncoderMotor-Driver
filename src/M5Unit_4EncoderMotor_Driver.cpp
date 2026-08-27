/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include "M5Unit_4EncoderMotor_Driver.hpp"

M5Unit4EncoderMotorDriver::M5Unit4EncoderMotorDriver()
    : M5EncoderMotorDriverBase(M5_UNIT_4ENCODER_MOTOR_DEFAULT_ADDRESS, 4)
{
}

bool M5Unit4EncoderMotorDriver::setSoftStartStop(bool enabled, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(REG_SOFT_START_STOP, value)) {
        return false;
    }

    const uint8_t mask = static_cast<uint8_t>(1U << channel);
    value              = enabled ? static_cast<uint8_t>(value | mask) : static_cast<uint8_t>(value & ~mask);
    return writeByte(REG_SOFT_START_STOP, value);
}

bool M5Unit4EncoderMotorDriver::getSoftStartStop(bool& enabled, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(REG_SOFT_START_STOP, value)) {
        return false;
    }
    enabled = (value & static_cast<uint8_t>(1U << channel)) != 0;
    return true;
}
