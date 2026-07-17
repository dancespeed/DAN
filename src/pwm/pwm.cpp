#include "pwm.hpp"
#include "pwm_config.hpp"

#include "eventbus/eventbus.hpp"
#include "system/system.hpp"

#include "drivers/pwm_driver.hpp"

namespace
{
    ChannelRuntime channels[PWMConfig::ChannelCount];

    uint8_t ToIndex(ChannelId channel)
    {
        return static_cast<uint8_t>(channel);
    }

    bool IsValidChannel(ChannelId channel)
    {
        return ToIndex(channel) < PWMConfig::ChannelCount;
    }

    uint16_t CalculateBrightness(uint8_t index, uint8_t step)
    {
        const ChannelConfig& config = PWMConfig::Channels[index];

        if (step == 0)
        {
            return 0;
        }

        if (step >= config.maxStep)
        {
            return config.maxBrightness;
        }

        return channels[index].stepBrightness * step;
    }

    void PublishChannelStateChanged(ChannelId channel)
    {
        const uint8_t index = ToIndex(channel);

        Event event
        {
            EventType::ChannelStateChanged,
            EventTarget::Broadcast,
            static_cast<uint8_t>(channel),
            channels[index].currentBrightness
        };

        EventBus::Publish(event);
    }

    void SetChannelStep(ChannelId channel, uint8_t step)
    {
        if (!IsValidChannel(channel))
        {
            return;
        }

        const uint8_t index = ToIndex(channel);
        const ChannelConfig& config = PWMConfig::Channels[index];
        ChannelRuntime& state = channels[index];

        if (step == 0)
        {
            // Выключаем канал, но сохраняем последний активный currentStep.
            state.enabled = false;
            state.targetBrightness = 0;
        }
        else
        {
            if (step > config.maxStep)
            {
                step = config.maxStep;
            }

            state.enabled = true;
            state.currentStep = step;
            state.targetBrightness = CalculateBrightness(index, step);
        }

        if (!config.smoothEnable)
        {
            if (!PWMDriver::Set(config.driverChannel, state.targetBrightness))
            {
            return;
            }

        state.currentBrightness = state.targetBrightness;

        PublishChannelStateChanged(channel);
        }
    }

    void ProcessTransition(uint8_t index)
    {
        ChannelRuntime& state = channels[index];
        const ChannelConfig& config = PWMConfig::Channels[index];

        if (state.currentBrightness == state.targetBrightness)
        {
            return;
        }

        const uint32_t now = System::GetTickMs();

        if ((now - state.lastUpdateMs) < 1)
        {
            return;
        }

        state.lastUpdateMs = now;

        uint16_t nextBrightness = state.currentBrightness;

        if (nextBrightness < state.targetBrightness)
        {
            nextBrightness++;
        }
        else
        {
            nextBrightness--;
        }

        if (!PWMDriver::Set(config.driverChannel, nextBrightness))
        {
            return;
        }

        state.currentBrightness = nextBrightness;

        if (state.currentBrightness == state.targetBrightness)
        {
            PublishChannelStateChanged(static_cast<ChannelId>(index));
        }
    }
}

namespace PWM
{
    void Init()
    {
        for (uint8_t i = 0; i < PWMConfig::ChannelCount; i++)
        {
            const ChannelConfig& config = PWMConfig::Channels[i];
            ChannelRuntime& state = channels[i];

            state.enabled = false;
            state.currentStep = 1;

            state.currentBrightness = 0;
            state.targetBrightness = 0;

            if (config.maxStep == 0)
            {
                state.stepBrightness = config.maxBrightness;
            }
            else
            {
                state.stepBrightness = config.maxBrightness / config.maxStep;
            }

            state.lastUpdateMs = System::GetTickMs();

            PWMDriver::Set (config.driverChannel, 0);
        }
    }

    void Process()
    {
        for (uint8_t i = 0; i < PWMConfig::ChannelCount; i++)
        {
            ProcessTransition(i);
        }
    }

    void HandleEvent(const Event& event)
    {
        if (event.type != EventType::SetChannelState)
        {
            return;
        }

        if ((event.target != EventTarget::PWM) && (event.target != EventTarget::Broadcast))
        {
            return;
        }

        const ChannelId channel = static_cast<ChannelId>(event.object);

        const uint8_t step = static_cast<uint8_t>(event.value);

        SetChannelStep(channel, step);
    }

    bool IsEnabled(ChannelId channel)
    {
        if (!IsValidChannel(channel))
        {
            return false;
        }

        return channels[ToIndex(channel)].enabled;
    }

    uint8_t GetCurrentStep(ChannelId channel)
    {
        if (!IsValidChannel(channel))
        {
            return 0;
        }

        return channels[ToIndex(channel)].currentStep;
    }

    uint8_t GetMaxStep(ChannelId channel)
    {
        if (!IsValidChannel(channel))
        {
            return 0;
        }

        return PWMConfig::Channels[ToIndex(channel)].maxStep;
    }

    ChannelType GetChannelType(ChannelId channel)
    {
        if (!IsValidChannel(channel))
        {
            return ChannelType::OnOff;
        }

        return PWMConfig::Channels[ToIndex(channel)].type;
}

    uint16_t GetCurrentBrightness(ChannelId channel)
    {
        if (!IsValidChannel(channel))
        {
            return 0;
        }

        return channels[ToIndex(channel)].currentBrightness;
    }
}