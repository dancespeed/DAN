#include "logic.hpp"

#include "eventbus/eventbus.hpp"

#include "pwm/pwm.hpp"
#include "pwm/pwm_types.hpp"

namespace
{
    void PublishSetChannelState(
        ChannelId channel,
        uint8_t step
    )
    {
        const Event event
        {
            EventType::SetChannelState,
            EventTarget::PWM,
            static_cast<uint8_t>(channel),
            step
        };

        EventBus::Publish(event);
    }

    void HandleClick(ChannelId channel)
    {
        uint8_t step =
            PWM::GetCurrentStep(channel);

        if (!PWM::IsEnabled(channel))
        {
            PublishSetChannelState(
                channel,
                step
            );

            return;
        }

        const uint8_t maxStep =
            PWM::GetMaxStep(channel);

        if (step < maxStep)
        {
            step++;
        }
        else
        {
            step = 1;
        }

        PublishSetChannelState(
            channel,
            step
        );
    }

    void HandleHold(ChannelId channel)
    {
        PublishSetChannelState(
            channel,
            0
        );
    }
}

namespace Logic
{
    void Init()
    {
    }

    void HandleEvent(
        const Event& event
    )
    {
        if (event.type !=
            EventType::UserCommand)
        {
            return;
        }

        if ((event.target !=
                EventTarget::Logic) &&
            (event.target !=
                EventTarget::Broadcast))
        {
            return;
        }

        const ChannelId channel =
            static_cast<ChannelId>(
                event.object
            );

        const CommandType command =
            static_cast<CommandType>(
                event.value
            );

        switch (command)
        {
            case CommandType::Click:
            {
                HandleClick(channel);
                break;
            }

            case CommandType::Hold:
            {
                HandleHold(channel);
                break;
            }

            case CommandType::None:
            default:
            {
                break;
            }
        }
    }
}