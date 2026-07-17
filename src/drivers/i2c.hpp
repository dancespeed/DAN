#pragma once

#include <stdint.h>

namespace I2C
{
    void Init();

    bool StartWrite(uint8_t address7Bit);
    bool StartRead(uint8_t address7Bit);

    bool Write(uint8_t data);

    bool ReadNack(uint8_t& data);

    void Stop();
}