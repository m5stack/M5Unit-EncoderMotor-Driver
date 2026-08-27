/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef M5_UNIT_ENCODER_MOTOR_TYPES_HPP
#define M5_UNIT_ENCODER_MOTOR_TYPES_HPP

#include <Arduino.h>

enum m5_unit_encoder_motor_driver_t : uint8_t {
    M5_UNIT_ENCODER_MOTOR_DRIVER = 0,
    M5_UNIT_4ENCODER_MOTOR_DRIVER,
};

// Product selectors passed to the M5UnitEncoderMotor constructor.
#define M5Unit_EncoderMotor_Driver  M5_UNIT_ENCODER_MOTOR_DRIVER
#define M5Unit_4EncoderMotor_Driver M5_UNIT_4ENCODER_MOTOR_DRIVER

enum m5_unit_encoder_motor_mode_t : uint8_t {
    M5_UNIT_ENCODER_MOTOR_MODE_NORMAL = 0,
    M5_UNIT_ENCODER_MOTOR_MODE_POSITION,
    M5_UNIT_ENCODER_MOTOR_MODE_SPEED,
};

enum m5_unit_encoder_motor_encoder_mode_t : uint8_t {
    M5_UNIT_ENCODER_MOTOR_ENCODER_AB = 0,
    M5_UNIT_ENCODER_MOTOR_ENCODER_BA,
};

struct M5UnitEncoderMotorPID {
    uint8_t kp;
    uint8_t ki;
    uint8_t kd;
};

struct M5UnitEncoderMotorConfig {
    m5_unit_encoder_motor_mode_t mode;
    M5UnitEncoderMotorPID positionPID;
    int32_t positionPoint;
    int8_t positionMaxSpeed;
    M5UnitEncoderMotorPID speedPID;
    int8_t speedPoint;
};

static constexpr uint8_t M5_UNIT_ENCODER_MOTOR_DEFAULT_ADDRESS  = 0x2F;
static constexpr uint8_t M5_UNIT_4ENCODER_MOTOR_DEFAULT_ADDRESS = 0x24;
static constexpr uint8_t M5_UNIT_ENCODER_MOTOR_MIN_ADDRESS      = 0x08;
static constexpr uint8_t M5_UNIT_ENCODER_MOTOR_MAX_ADDRESS      = 0x77;

#endif
