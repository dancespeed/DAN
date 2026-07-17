#include "pwm_config.hpp"

namespace PWMConfig
{
    const ChannelConfig Channels[ChannelCount] =
    {
        {
            ChannelId::Channel1,
            ChannelType::Step,
            4,      // PCA9685 LED4 = STURM_LH
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Channel2,
            ChannelType::Step,
            2,      // PCA9685 LED2 = STURM_RH
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Channel3,
            ChannelType::Step,
            3,      // PCA9685 LED3 = SIDE
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Channel4,
            ChannelType::Step,
            7,      // PCA9685 LED7 = CEIL
            4095,
            4,
            true,
            true
        }
    };
}