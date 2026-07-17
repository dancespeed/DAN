#pragma once

#include "input_types.hpp"

namespace Input
{
    bool Init();

    void Process();

    bool IsPressed(ButtonId button);
}