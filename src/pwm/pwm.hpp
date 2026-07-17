#pragma once

#include <stdint.h>

#include "event/event.hpp"
#include "pwm_types.hpp"

namespace PWM
{
    void Init();
    void Process();

    void HandleEvent(const Event& event);

    bool IsEnabled(ChannelId channel);

    uint8_t GetCurrentStep(ChannelId channel);
    uint8_t GetMaxStep(ChannelId channel);

    uint16_t GetCurrentBrightness(ChannelId channel);
}