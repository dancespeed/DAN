#pragma once

#include <stdint.h>

#include "event/event.hpp"

namespace Sequencer
{
    enum class Command : uint8_t
    {
        PublishEvent
    };

    struct SequenceStep
    {
        Command command;
        Event   event;
    };

    void Init();

    void Load(const SequenceStep* sequence, uint8_t size);

    void HandleEvent(const Event& event);

    void Update();
}