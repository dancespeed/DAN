#include "i2c.hpp"
#include "i2c_config.hpp"

#include <avr/io.h>
#include <util/twi.h>

namespace I2C
{
    void Stop();
}

namespace
{
    bool WaitForComplete()
    {
        uint16_t timeout =
            I2CConfig::TimeoutIterations;

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

    bool Start()
    {
        TWCR =
            (1 << TWINT) |
            (1 << TWSTA) |
            (1 << TWEN);

        if (!WaitForComplete())
        {
            I2C::Stop();
            return false;
        }

        const uint8_t status =
            GetStatus();

        if ((status != TW_START) &&
            (status != TW_REP_START))
        {
            I2C::Stop();
            return false;
        }

        return true;
    }

    bool SendAddress(
        uint8_t address7Bit,
        bool read
    )
    {
        TWDR = static_cast<uint8_t>(
            (address7Bit << 1) |
            (read ? 1 : 0)
        );

        TWCR =
            (1 << TWINT) |
            (1 << TWEN);

        if (!WaitForComplete())
        {
            I2C::Stop();
            return false;
        }

        const uint8_t expectedStatus =
            read
                ? TW_MR_SLA_ACK
                : TW_MT_SLA_ACK;

        if (GetStatus() != expectedStatus)
        {
            I2C::Stop();
            return false;
        }

        return true;
    }
}

namespace I2C
{
    void Init()
    {
        // Предделитель TWI = 1.
        TWSR &=
            ~((1 << TWPS1) |
              (1 << TWPS0));

        // SCL =
        // F_CPU / (16 + 2 * TWBR * prescaler)
        TWBR = static_cast<uint8_t>(
            ((I2CConfig::CpuFrequencyHz /
              I2CConfig::BusFrequencyHz) -
             16UL) /
            2UL
        );

        TWCR = (1 << TWEN);
    }

    bool StartWrite(uint8_t address7Bit)
    {
        if (!Start())
        {
            return false;
        }

        return SendAddress(
            address7Bit,
            false
        );
    }

    bool StartRead(uint8_t address7Bit)
    {
        if (!Start())
        {
            return false;
        }

        return SendAddress(
            address7Bit,
            true
        );
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

        if (GetStatus() !=
            TW_MT_DATA_ACK)
        {
            Stop();
            return false;
        }

        return true;
    }

    bool ReadNack(uint8_t& data)
    {
        // TWEA не устанавливаем:
        // после принятого байта ведущий отвечает NACK.
        TWCR =
            (1 << TWINT) |
            (1 << TWEN);

        if (!WaitForComplete())
        {
            Stop();
            return false;
        }

        if (GetStatus() !=
            TW_MR_DATA_NACK)
        {
            Stop();
            return false;
        }

        data = TWDR;

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