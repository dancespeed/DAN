#include <Arduino.h>

#include "drivers/i2c.hpp"
#include "drivers/pwm_driver.hpp"

constexpr uint8_t TestChannel = 4;

void setup()
{
    Serial.begin(115200);

    const bool initialized =
        PWMDriver::Init();

    if (!initialized)
    {
        Serial.println(
            "PWM driver initialization failed"
        );

        return;
    }

    Serial.println(
        "PWM driver initialized"
    );

    const bool channelSet =
        PWMDriver::Set(TestChannel, 4095);

    if (channelSet)
    {
        Serial.println(
            "PCA9685 TestChannel set to maximum"
        );
    }
    else
    {
        Serial.println(
            "PCA9685 TestChannel write failed"
        );
    }
}

void loop()
{
}