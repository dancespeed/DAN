#pragma once

#include "event/event.hpp"

#include <stdint.h>

namespace EventBus
{
    void Init();

    bool Publish(const Event& event);

    bool Get(Event& event);

    bool HasEvents();

    uint8_t Count();

    void Clear();
}