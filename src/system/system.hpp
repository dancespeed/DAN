#pragma once

#include <stdint.h>

namespace System
{
    void Init();
    void Process();

    uint32_t GetTickMs();
}