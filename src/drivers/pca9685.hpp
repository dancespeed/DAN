#pragma once

#include <stdint.h>

namespace PCA9685
{
    bool Init();

    bool Set(uint8_t channel,
             uint16_t brightness);

    bool SetAll(uint16_t brightness);
}