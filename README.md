# M5Unit EncoderMotor Driver

## Overview

### SKU: U232 & U233

This Arduino library supports the M5Stack Unit EncoderMotor Driver (U232) and Unit 4EncoderMotor Driver (U233).
Both products use the public `M5UnitEncoderMotor` class. Product-specific protocol behavior is kept in separate
single-channel and four-channel drivers.

The library provides I2C APIs for open-loop PWM control, encoder feedback, speed and position closed-loop control,
input voltage and current monitoring, RGB LED control, and persistent device settings.

## Supported Products

| Product | SKU | Constructor selector | Default I2C address | Motor channels |
| :--- | :---: | :--- | :---: | :---: |
| Unit EncoderMotor Driver | U232 | `M5Unit_EncoderMotor_Driver` | `0x2F` | 1 |
| Unit 4EncoderMotor Driver | U233 | `M5Unit_4EncoderMotor_Driver` | `0x24` | 4 |

Motor channel arguments are zero based. U232 uses channel `0`; U233 supports channels `0` through `3`.

## Related Links

- [Unit EncoderMotor Driver (U232) Document & Datasheet](https://docs.m5stack.com/en/products/sku/U232)
- [Unit 4EncoderMotor Driver (U233) Document & Datasheet](https://docs.m5stack.com/en/products/sku/U233)

## Required Libraries

- Arduino `Wire` library, included with the Arduino-ESP32 core

No third-party Arduino library is required.

## Installation

### Arduino IDE

Download this repository as a ZIP file and select **Sketch > Include Library > Add .ZIP Library**. After installation,
include the library with:

```cpp
#include <M5Unit-EncoderMotor-Driver.h>
```

### PlatformIO

Add the Git repository to `platformio.ini`:

```ini
lib_deps =
    https://gitlab.m5stack.com/m5-arduino-library/M5Unit-EncoderMotor-Driver.git
```

## Product Selection

Pass the matching product selector to the constructor:

```cpp
#include <M5Unit-EncoderMotor-Driver.h>

M5UnitEncoderMotor motor(M5Unit_EncoderMotor_Driver);
M5UnitEncoderMotor motors(M5Unit_4EncoderMotor_Driver);
```

Passing `0` as the address selects the default address for the chosen product:

```cpp
motor.begin(&Wire, 0, 21, 22, 400000);
motors.begin(&Wire, 0, 21, 22, 400000);
```

`begin()` arguments are the `TwoWire` instance, I2C address, SDA pin, SCL pin, and I2C frequency. The examples use
GPIO 21 for SDA and GPIO 22 for SCL; change these values for boards with different I2C pins.

## Speed Control

Speed values are signed encoder-count increments measured over 20 ms. They are not RPM, PWM duty, or linear speed.
The supported protocol range is `-127` through `127`.

The driver API performs only the operation requested by the caller. Application startup policy, such as stopping
open-loop output before entering Speed mode, remains explicit in the sketch:

```cpp
const M5UnitEncoderMotorPID speedPID = {10, 1, 15};

motor.setMotorRun(true);  // U232 only
motor.setMotorMode(M5_UNIT_ENCODER_MOTOR_MODE_NORMAL);
motor.setPWMDuty(0);
motor.runSpeed(15, speedPID);
```

`runSpeed(speedPoint)` writes the target and enters Speed mode without changing the existing PID. The overload that
accepts `M5UnitEncoderMotorPID` writes the supplied PID and target before changing the mode.

`getSpeed()` returns the raw signed encoder delta exposed by the product protocol. When the persistent encoder mode is
set to `BA`, the raw speed sign can be opposite the normalized closed-loop target; for example, a `+15` target can
produce a `-15` raw speed reading. The library does not hide or invert this protocol value.

## Position Control

Position values are signed 32-bit encoder counts. `runPosition()` writes the requested position PID, target position,
and maximum speed before entering Position mode. It does not clear the current encoder value or stop the motor first.

```cpp
const M5UnitEncoderMotorPID positionPID = {3, 1, 15};
motor.runPosition(1000, 20, positionPID);
```

## API Overview

| Group | Public methods |
| :--- | :--- |
| Device | `begin`, `isConnected`, `driverType`, `motorCount`, `getAddress`, `getDefaultAddress`, `getFirmwareVersion` |
| PWM and feedback | `setPWMDuty`, `getPWMDuty`, `setEncoder`, `clearEncoder`, `getEncoder`, `getSpeed` |
| Motor configuration | `setConfig`, `getConfig`, `setMotorMode`, `getMotorMode` |
| Position control | `setPositionPID`, `getPositionPID`, `setPositionPoint`, `getPositionPoint`, `setPositionMaxSpeed`, `getPositionMaxSpeed`, `runPosition` |
| Speed control | `setSpeedPID`, `getSpeedPID`, `setSpeedPoint`, `getSpeedPoint`, `runSpeed`, `stop` |
| Power monitoring | `getMotorCurrent`, `getVinADC12`, `getVinADC8`, `getVinMillivolts`, `getVinVoltage` |
| Device settings | `setEncoderMode`, `getEncoderMode`, `setSoftStartStop`, `getSoftStartStop`, `setI2CAddress`, `getI2CAddress` |
| U232 motor power | `setMotorRun`, `getMotorRun`, `setMotorSleep` |
| RGB and identity | `setRGBColor`, `getRGBColor`, `setRGBBrightness`, `getRGBBrightness`, `getUID` |

All I2C methods return `true` on success and `false` on invalid arguments, unsupported product behavior, or an I2C
transaction failure.

## Product-Specific Behavior

- `setSoftStartStop()` writes a Boolean value on U232 and updates the selected channel bit on U233.
- `setMotorRun()` and `setMotorSleep()` are supported only by U232 and return `false` for U233.
- Both products reinitialize their I2C peripheral without restarting after an address change. The U232 protocol asks
  the host to wait about 300 ms before using the new address; U233 does not specify a fixed wait. The API returns
  immediately after sending the command, so application code is responsible for any required delay.
- Changing the encoder mode restarts the product. The setting is stored in flash and should not be written repeatedly.
- RGB color and brightness are independent APIs. `setRGBColor()` does not overwrite the current brightness.
- Current monitoring reports product input current. U233 does not provide separate per-channel current measurements.

## Examples

- `UnitEncoderMotor_Speed` runs the U232 motor in closed-loop Speed mode.
- `Unit4EncoderMotor_Speed` runs all four U233 motors in closed-loop Speed mode.

Both examples cycle the onboard RGB LED through red, green, blue, and white during setup, then continuously print
speed, input voltage, and current data.

## Versioning

The current library version is `0.1.0`. Releases follow [Semantic Versioning](https://semver.org/):

- `MAJOR` changes indicate incompatible public API changes.
- `MINOR` changes add backward-compatible functionality.
- `PATCH` changes contain backward-compatible fixes.

For each release, keep the versions in `library.properties` and `library.json` identical, update `CHANGELOG.md`, and
create a Git tag named `vMAJOR.MINOR.PATCH`.

## License

- [M5Unit EncoderMotor Driver - MIT](LICENSE)
