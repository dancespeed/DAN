#include "pwm_driver.hpp"
#include "pwm_driver_config.hpp"

#include "i2c.hpp"
#include "pca9685.hpp"

#include <avr/io.h>

namespace PWMDriver
{
    void DisableOutputs()
    {
        PORTD |=
            (1 << PWMDriverConfig::OutputEnableBit);
    }

    void EnableOutputs()
    {
        PORTD &=
            ~(1 << PWMDriverConfig::OutputEnableBit);
    }

    bool Init()
    {
        DisableOutputs();

        // Сначала устанавливаем безопасный HIGH,
        // затем переводим PD5 в режим выхода.
        PORTD |=
            (1 << PWMDriverConfig::OutputEnableBit);

        DDRD |=
            (1 << PWMDriverConfig::OutputEnableBit);

        // I2C::Init() только конфигурирует TWI-регистры.
        I2C::Init();

        if (!PCA9685::Init())
        {
            return false;
        }

        if (!PCA9685::SetAll(0))
        {
            return false;
        }

        EnableOutputs();

        return true;
    }

    bool Set(uint8_t driverChannel,
             uint16_t brightness)
    {
        return PCA9685::Set(
            driverChannel,
            brightness
        );
    }
}