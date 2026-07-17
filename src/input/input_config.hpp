#pragma once

#include <avr/io.h>
#include <stdint.h>

namespace InputConfig
{
    constexpr uint8_t Pcf8574Address = 0x20;

    // PCF8574 INT → PD2 / INT0.
    constexpr uint8_t PcfInterruptBit = PD2;

    // Прямая кнопка CEIL → PC0.
    constexpr uint8_t CeilButtonBit = PC0;

    constexpr uint16_t DebounceTimeMs = 50;

    // Подтверждённое отпускание до этого времени:
    // CommandType::Click.
    constexpr uint16_t ClickMaxTimeMs = 1500;

    // Непрерывное удержание не менее этого времени:
    // CommandType::Hold.
    constexpr uint16_t HoldTimeMs = 2000;
}