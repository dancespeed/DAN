#include "pcf8574_input.hpp"

#include "input_config.hpp"

#include "drivers/i2c.hpp"

namespace PCF8574Input
{
    bool Init()
    {
        if (!I2C::StartWrite(
                InputConfig::Pcf8574Address
            ))
        {
            return false;
        }

        // Для PCF8574 запись единицы освобождает линию,
        // после чего её можно использовать как вход.
        if (!I2C::Write(0xFF))
        {
            I2C::Stop();
            return false;
        }

        I2C::Stop();

        return true;
    }

    bool Read(uint8_t& value)
    {
        if (!I2C::StartRead(
                InputConfig::Pcf8574Address
            ))
        {
            return false;
        }

        if (!I2C::ReadNack(value))
        {
            I2C::Stop();
            return false;
        }

        I2C::Stop();

        return true;
    }
}