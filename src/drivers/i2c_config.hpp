#pragma once

#include <stdint.h>

namespace I2CConfig
{
    constexpr uint32_t CpuFrequencyHz = 16000000UL;
    constexpr uint32_t BusFrequencyHz = 100000UL;

    // Ограничивает ожидание завершения операции TWI.
    constexpr uint16_t TimeoutIterations = 20000;
}