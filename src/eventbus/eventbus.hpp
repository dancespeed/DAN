#pragma once

#include "event/event.hpp"
#include "message/module_id.hpp"

#include <stdint.h>

namespace EventBus
{
    void Init();

    bool Publish(
        ModuleId sender,
        const Event& event
    );

    bool Get(Event& event);

    bool HasEvents();

    uint8_t Count();

    void Clear();
}
