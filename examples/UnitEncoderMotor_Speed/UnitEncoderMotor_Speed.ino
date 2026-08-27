/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include <M5Unit-EncoderMotor-Driver.h>

M5UnitEncoderMotor motor(M5Unit_EncoderMotor_Driver);

static constexpr int8_t I2C_SDA                  = 21;
static constexpr int8_t I2C_SCL                  = 22;
static constexpr uint32_t I2C_FREQUENCY          = 400000;
static constexpr int8_t TARGET_SPEED             = 15;
static constexpr M5UnitEncoderMotorPID SPEED_PID = {10, 1, 15};

static void halt(const char* message)
{
    Serial.println(message);
    while (true) {
        delay(1000);
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    while (!motor.begin(&Wire, 0, I2C_SDA, I2C_SCL, I2C_FREQUENCY)) {
        Serial.println("Unit EncoderMotor not found. Retrying...");
        delay(1000);
    }

    if (!motor.setRGBBrightness(127)) {
        halt("Failed to set RGB brightness.");
    }
    delay(10);

    const uint8_t colors[][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 255},
    };
    for (const auto& color : colors) {
        if (!motor.setRGBColor(color[0], color[1], color[2])) {
            halt("Failed to set RGB color.");
        }
        delay(500);
    }

    // Application startup policy: enable the driver, stop open-loop output,
    // configure the speed controller, then enter Speed mode.
    if (!motor.setMotorRun(true) || !motor.setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_NORMAL) || !motor.setPWMDuty(0) ||
        !motor.runSpeed(TARGET_SPEED, SPEED_PID)) {
        halt("Failed to start the motor in Speed mode.");
    }

    Serial.printf("Target speed: %d encoder counts / 20 ms\r\n", TARGET_SPEED);
}

void loop()
{
    int8_t measuredSpeed = 0;
    if (motor.getSpeed(measuredSpeed)) {
        Serial.printf("Measured speed: %d encoder counts / 20 ms\r\n", measuredSpeed);
    } else {
        Serial.println("Failed to read motor speed.");
    }

    float voltage = 0.0f;
    float current = 0.0f;
    if (motor.getVinVoltage(voltage) && motor.getMotorCurrent(current)) {
        Serial.printf("VIN: %.3f V, current: %.3f A\r\n", voltage, current);
    } else {
        Serial.println("Failed to read voltage or current.");
    }
    delay(200);
}
