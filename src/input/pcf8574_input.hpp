#pragma once

#include <stdint.h>

namespace PCF8574Input
{
    bool Init();

    bool Read(uint8_t& value);
}