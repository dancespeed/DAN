#include "pca9685.hpp"

#include "i2c.hpp"

#include <Arduino.h>

namespace
{
    constexpr uint8_t I2cAddress = 0x40;

    constexpr uint8_t Mode1Register = 0x00;
    constexpr uint8_t Mode2Register = 0x01;
    constexpr uint8_t Led0OnLowRegister = 0x06;
    constexpr uint8_t PrescaleRegister = 0xFE;

    constexpr uint8_t ChannelCount = 16;
    constexpr uint16_t MaxBrightness = 4095;

    constexpr uint8_t Mode1SleepBit = 4;
    constexpr uint8_t Mode1AutoIncrementBit = 5;

    constexpr uint8_t Mode2OutputDriverBit = 2;

    // Около 1 кГц при внутреннем генераторе PCA9685 25 МГц.
    constexpr uint8_t PrescaleValue = 5;

    bool WriteRegister(
        uint8_t registerAddress,
        uint8_t value
    )
    {
        if (!I2C::StartWrite(I2cAddress))
        {
            return false;
        }

        if (!I2C::Write(registerAddress))
        {
            I2C::Stop();
            return false;
        }

        if (!I2C::Write(value))
        {
            I2C::Stop();
            return false;
        }

        I2C::Stop();

        return true;
    }

    bool WriteRegisters(
        uint8_t startRegister,
        const uint8_t* data,
        uint8_t length
    )
    {
        if (data == nullptr)
        {
            return false;
        }

        if (!I2C::StartWrite(I2cAddress))
        {
            return false;
        }

        if (!I2C::Write(startRegister))
        {
            I2C::Stop();
            return false;
        }

        for (uint8_t i = 0; i < length; i++)
        {
            if (!I2C::Write(data[i]))
            {
                I2C::Stop();
                return false;
            }
        }

        I2C::Stop();

        return true;
    }
}

namespace PCA9685
{
    bool Init()
    {
        // Переводим PCA9685 в SLEEP,
        // потому что PRESCALE разрешено менять только во сне.
        if (!WriteRegister(
                Mode1Register,
                static_cast<uint8_t>(1 << Mode1SleepBit)
            ))
        {
            return false;
        }

        if (!WriteRegister(
                PrescaleRegister,
                PrescaleValue
            ))
        {
            return false;
        }

        // Выходим из SLEEP и включаем Auto Increment.
        if (!WriteRegister(
                Mode1Register,
                static_cast<uint8_t>(
                    1 << Mode1AutoIncrementBit
                )
            ))
        {
            return false;
        }

        // После выхода из SLEEP генератору нужно время на запуск.
        delayMicroseconds(500);

        // Totem-pole output driver.
        if (!WriteRegister(
                Mode2Register,
                static_cast<uint8_t>(
                    1 << Mode2OutputDriverBit
                )
            ))
        {
            return false;
        }

        return true;
    }

    bool Set(
        uint8_t channel,
        uint16_t brightness
    )
    {
        if (channel >= ChannelCount)
        {
            return false;
        }

        if (brightness > MaxBrightness)
        {
            brightness = MaxBrightness;
        }

        const uint8_t firstRegister =
            static_cast<uint8_t>(
                Led0OnLowRegister + channel * 4
            );

        const uint8_t data[4]
        {
            0x00, // LED_ON_L
            0x00, // LED_ON_H
            static_cast<uint8_t>(
                brightness & 0xFF
            ),
            static_cast<uint8_t>(
                (brightness >> 8) & 0x0F
            )
        };

        return WriteRegisters(
            firstRegister,
            data,
            sizeof(data)
        );
    }

    bool SetAll(uint16_t brightness)
    {
        if (brightness > MaxBrightness)
        {
            brightness = MaxBrightness;
        }

        for (uint8_t channel = 0;
             channel < ChannelCount;
             channel++)
        {
            if (!Set(channel, brightness))
            {
                return false;
            }
        }

        return true;
    }
}