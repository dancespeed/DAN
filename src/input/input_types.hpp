#pragma once

#include <stdint.h>

enum class ButtonId : uint8_t
{
    None = 0,

    SturmLh,
    SturmRh,
    Side,
    Res1,
    Res2,
    AromaIon,
    Smart,
    Exhst,
    Ceil,

    Count
};

struct ButtonRuntime
{
    bool rawPressed;
    bool stablePressed;

    bool holdPublished;

    uint32_t rawChangedMs;
    uint32_t pressedMs;
};