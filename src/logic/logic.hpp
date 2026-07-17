#pragma once

#include "event/event.hpp"

namespace Logic
{
    void Init();

    void HandleEvent(const Event& event);
}