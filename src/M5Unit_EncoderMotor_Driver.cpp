/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include "M5Unit_EncoderMotor_Driver.hpp"

M5UnitEncoderMotorSingleDriver::M5UnitEncoderMotorSingleDriver()
    : M5EncoderMotorDriverBase(M5_UNIT_ENCODER_MOTOR_DEFAULT_ADDRESS, 1)
{
}

bool M5UnitEncoderMotorSingleDriver::setSoftStartStop(bool enabled, uint8_t channel)
{
    return isValidChannel(channel) && writeByte(REG_SOFT_START_STOP, enabled ? 1 : 0);
}

bool M5UnitEncoderMotorSingleDriver::getSoftStartStop(bool& enabled, uint8_t channel)
{
    uint8_t value = 0;
    if (!isValidChannel(channel) || !readByte(REG_SOFT_START_STOP, value)) {
        return false;
    }
    enabled = value != 0;
    return true;
}

bool M5UnitEncoderMotorSingleDriver::setMotorRun(bool run)
{
    return writeByte(REG_MOTOR_SLEEP, run ? 1 : 0);
}

bool M5UnitEncoderMotorSingleDriver::getMotorRun(bool& run)
{
    uint8_t value = 0;
    if (!readByte(REG_MOTOR_SLEEP, value)) {
        return false;
    }
    run = value != 0;
    return true;
}
