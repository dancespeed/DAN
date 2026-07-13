#pragma once

#include "pwm_types.hpp"

namespace PWMConfig
{
    constexpr uint8_t ChannelCount = static_cast<uint8_t>(ChannelId::Count);

    extern const ChannelConfig Channels[ChannelCount];
}