#pragma once

#include "event/event.hpp"

namespace Sequencer
{
    void Init();

    void HandleEvent(const Event& event);

    void Update();
}