/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include <M5Unit-EncoderMotor-Driver.h>

M5UnitEncoderMotor motors(M5Unit_4EncoderMotor_Driver);

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

    while (!motors.begin(&Wire, 0, I2C_SDA, I2C_SCL, I2C_FREQUENCY)) {
        Serial.println("Unit 4EncoderMotor not found. Retrying...");
        delay(1000);
    }

    if (!motors.setRGBBrightness(127)) {
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
        if (!motors.setRGBColor(color[0], color[1], color[2])) {
            halt("Failed to set RGB color.");
        }
        delay(500);
    }

    for (uint8_t channel = 0; channel < motors.motorCount(); ++channel) {
        // Application startup policy: stop open-loop output, configure the
        // speed controller, then enter Speed mode.
        if (!motors.setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_NORMAL, channel) || !motors.setPWMDuty(0, channel) ||
            !motors.runSpeed(TARGET_SPEED, SPEED_PID, channel)) {
            halt("Failed to start all motors in Speed mode.");
        }
    }

    Serial.printf("All motors target: %d encoder counts / 20 ms\r\n", TARGET_SPEED);
}

void loop()
{
    for (uint8_t channel = 0; channel < motors.motorCount(); ++channel) {
        int8_t measuredSpeed = 0;
        if (motors.getSpeed(measuredSpeed, channel)) {
            Serial.printf("M%d: %d  ", channel + 1, measuredSpeed);
        } else {
            Serial.printf("M%d: read error  ", channel + 1);
        }
    }
    Serial.println();

    float voltage = 0.0f;
    float current = 0.0f;
    if (motors.getVinVoltage(voltage) && motors.getMotorCurrent(current)) {
        Serial.printf("VIN: %.3f V, total current: %.3f A\r\n", voltage, current);
    } else {
        Serial.println("Failed to read voltage or current.");
    }
    delay(200);
}
