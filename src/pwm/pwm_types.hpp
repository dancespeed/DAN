#pragma once

#include <stdint.h>

enum class ChannelId : uint8_t
{
    Channel1 = 0,
    Channel2,
    Channel3,
    Channel4,

    Count
};

enum class ChannelType : uint8_t
{
    OnOff,
    Step
};

struct ChannelConfig
{
    ChannelId id;
    ChannelType type;

    uint16_t maxBrightness;
    uint8_t maxStep;

    bool smoothEnable;
    bool restoreEnable;
};

struct ChannelRuntime
{
    bool enabled;

    uint8_t currentStep;

    uint16_t currentBrightness;
    uint16_t targetBrightness;

    uint16_t stepBrightness;

    uint32_t lastUpdateMs;
};