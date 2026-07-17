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
            ChannelType::OnOff,
            5,      // PCA9685 LED5 = ION
            4095,
            1,
            false,
            true
        },

        {
            ChannelId::Aroma,
            ChannelType::OnOff,
            6,      // PCA9685 LED6 = AROMA
            4095,
            1,
            false,
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
            ChannelType::OnOff,
            8,      // PCA9685 LED8 = EXHST
            4095,
            1,
            false,
            true
        },

        {
            ChannelId::Smart,
            ChannelType::OnOff,
            9,      // PCA9685 LED9 = SMART
            4095,
            1,
            false,
            true
        },

        {
            ChannelId::Res1,
            ChannelType::OnOff,
            10,     // PCA9685 LED10 = RES1
            4095,
            1,
            false,
            true
        },

        {
            ChannelId::Res2,
            ChannelType::OnOff,
            11,     // PCA9685 LED11 = RES2
            4095,
            1,
            false,
            true
        }
    };
}