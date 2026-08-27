/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef M5_UNIT_4ENCODER_MOTOR_DRIVER_HPP
#define M5_UNIT_4ENCODER_MOTOR_DRIVER_HPP

#include "M5EncoderMotorDriverBase.hpp"

class M5Unit4EncoderMotorDriver : public M5EncoderMotorDriverBase {
public:
    M5Unit4EncoderMotorDriver();

    bool setSoftStartStop(bool enabled, uint8_t channel = 0) override;
    bool getSoftStartStop(bool& enabled, uint8_t channel = 0) override;
};

#endif
