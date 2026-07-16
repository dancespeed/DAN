#pragma once

#include <stdint.h>

namespace PWMDriver
{
    bool Init();

    void EnableOutputs();
    void DisableOutputs();

    bool Set(uint8_t driverChannel,
             uint16_t brightness);
}