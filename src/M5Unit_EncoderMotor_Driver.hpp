/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef M5_UNIT_ENCODER_MOTOR_SINGLE_DRIVER_HPP
#define M5_UNIT_ENCODER_MOTOR_SINGLE_DRIVER_HPP

#include "M5EncoderMotorDriverBase.hpp"

class M5UnitEncoderMotorSingleDriver : public M5EncoderMotorDriverBase {
public:
    M5UnitEncoderMotorSingleDriver();

    bool setSoftStartStop(bool enabled, uint8_t channel = 0) override;
    bool getSoftStartStop(bool& enabled, uint8_t channel = 0) override;
    bool setMotorRun(bool run) override;
    bool getMotorRun(bool& run) override;
};

#endif
