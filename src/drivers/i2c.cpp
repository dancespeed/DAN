#include "i2c.hpp"
#include "i2c_config.hpp"

#include <avr/io.h>
#include <util/twi.h>

namespace
{
    bool WaitForComplete()
    {
        uint16_t timeout = I2CConfig::TimeoutIterations;

        while ((TWCR & (1 << TWINT)) == 0)
        {
            if (timeout == 0)
            {
                return false;
            }

            timeout--;
        }

        return true;
    }

    uint8_t GetStatus()
    {
        return TWSR & 0xF8;
    }
}

namespace I2C
{
    void Init()
    {
        // Предделитель TWI = 1.
        TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

        // SCL = F_CPU / (16 + 2 * TWBR * prescaler)
        TWBR = static_cast<uint8_t>(
            ((I2CConfig::CpuFrequencyHz /
              I2CConfig::BusFrequencyHz) - 16UL) / 2UL
        );

        // Включаем аппаратный TWI.
        TWCR = (1 << TWEN);
    }

    bool StartWrite(uint8_t address7Bit)
    {
        // Формируем условие START.
        TWCR =
            (1 << TWINT) |
            (1 << TWSTA) |
            (1 << TWEN);

        if (!WaitForComplete())
        {
            Stop();
            return false;
        }

        const uint8_t startStatus = GetStatus();

        if ((startStatus != TW_START) &&
            (startStatus != TW_REP_START))
        {
            Stop();
            return false;
        }

        // Семибитный адрес сдвигается влево.
        // Младший бит 0 означает запись.
        TWDR = static_cast<uint8_t>(address7Bit << 1);

        TWCR =
            (1 << TWINT) |
            (1 << TWEN);

        if (!WaitForComplete())
        {
            Stop();
            return false;
        }

        if (GetStatus() != TW_MT_SLA_ACK)
        {
            Stop();
            return false;
        }

        return true;
    }

    bool Write(uint8_t data)
    {
        TWDR = data;

        TWCR =
            (1 << TWINT) |
            (1 << TWEN);

        if (!WaitForComplete())
        {
            Stop();
            return false;
        }

        if (GetStatus() != TW_MT_DATA_ACK)
        {
            Stop();
            return false;
        }

        return true;
    }

    void Stop()
    {
        TWCR =
            (1 << TWINT) |
            (1 << TWEN) |
            (1 << TWSTO);
    }
}