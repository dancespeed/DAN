#include "pwm_config.hpp"

namespace PWMConfig
{
    const ChannelConfig
        Channels[ChannelCount] =
    {
        {
            ChannelId::SturmLh,
            ChannelType::Step,
            4,      // PCA9685 LED4 = STURM_LH
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::SturmRh,
            ChannelType::Step,
            2,      // PCA9685 LED2 = STURM_RH
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Side,
            ChannelType::Step,
            3,      // PCA9685 LED3 = SIDE
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Ion,
            ChannelType::Step,
            5,      // PCA9685 LED5 = ION
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Aroma,
            ChannelType::Step,
            6,      // PCA9685 LED6 = AROMA
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Ceil,
            ChannelType::Step,
            7,      // PCA9685 LED7 = CEIL
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Exhst,
            ChannelType::Step,
            8,      // PCA9685 LED8 = EXHST
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Smart,
            ChannelType::Step,
            9,      // PCA9685 LED9 = SMART
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Res1,
            ChannelType::Step,
            10,     // PCA9685 LED10 = RES1
            4095,
            4,
            true,
            true
        },

        {
            ChannelId::Res2,
            ChannelType::Step,
            11,     // PCA9685 LED11 = RES2
            4095,
            4,
            true,
            true
        }
    };
}