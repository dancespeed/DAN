#include "pwm_config.hpp"

namespace PWMConfig
{
    const ChannelConfig Channels[ChannelCount] = 
    {
        {
            ChannelId::Channel1,
            ChannelType::Step,
            4095,
            4,
            true,
            true
        },
        {
            ChannelId::Channel2,
            ChannelType::Step,
            4095,
            4,
            true,
            true
        },
        {
            ChannelId::Channel3,
            ChannelType::Step,
            4095,
            4,
            true,
            true
        },
        {
            ChannelId::Channel4,
            ChannelType::Step,
            4095,
            4,
            true,
            true
        }
    };
}