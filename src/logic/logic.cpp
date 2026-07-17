#include <Arduino.h>

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
        const ChannelType type = PWM::GetChannelType(channel);

        /*

        Serial.print ("HandleClick channel: ");
        Serial.print (static_cast<uint8_t>(channel));

        Serial.print (", type: ");
        Serial.println (static_cast<uint8_t>(type));

        if (type == ChannelType::OnOff)
        {
            const bool enabled = PWM::IsEnabled(channel);

            const uint8_t targetStep = enabled ? 0 : 1;

            Serial.print ("OnOff enabled: ");
            Serial.print (enabled);

            Serial.print (", targetStep: ");
            Serial.println (targetStep);

            PublishSetChannelState(channel, targetStep);

            return;
        }
        */

        if (type == ChannelType::OnOff)
        {
            const uint8_t targetStep =
            PWM::IsEnabled(channel)
                ? 0
                : 1;

            PublishSetChannelState(channel, targetStep);

            return;
        }

        uint8_t step = PWM::GetCurrentStep(channel);

        if (!PWM::IsEnabled(channel))
        {
            PublishSetChannelState(channel, step);

            return;
        }

        const uint8_t maxStep = PWM::GetMaxStep(channel);

        if (step < maxStep)
        {
            step++;
        }
        else
        {
            step = 1;
        }

        PublishSetChannelState(channel, step);

    }

    void HandleHold(ChannelId channel)
    {
        PublishSetChannelState(channel, 0);
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
        /*
        Serial.print ("Logic event type: ");
        Serial.print (static_cast<uint8_t>(event.type));

        Serial.print (", target: ");
        Serial.print (static_cast<uint8_t>(event.target));

        Serial.print (", object: ");
        Serial.print (event.object);

        Serial.print (", value: ");
        Serial.println (event.value);

        if (event.type != EventType::UserCommand)
        {
            return;
        }
        */

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

        /*
        Serial.print ("UserCommand channel: ");
        Serial.print (static_cast<uint8_t>(channel));

        Serial.print (", command: ");
        Serial.println (static_cast<uint8_t>(command));
        */

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