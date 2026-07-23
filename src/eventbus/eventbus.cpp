#include "eventbus.hpp"
#include "eventbus_config.hpp"

#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "message/message_bus_config.hpp"
#include "message/module_id.hpp"

namespace
{
    Event queue[EventBusConfig::QueueSize];

    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t count = 0;

    ModuleId ConvertTarget(EventTarget target)
    {
        switch (target)
        {
            case EventTarget::Broadcast:
                return Module::Broadcast;

            case EventTarget::System:
                return Module::System;

            case EventTarget::Button:
                return Module::Button;

            case EventTarget::PWM:
                return Module::PWM;

            case EventTarget::EEPROM:
                return Module::EEPROM;

            case EventTarget::CAN:
                return Module::CAN;

            case EventTarget::Logic:
                return Module::Logic;

            case EventTarget::Sequencer:
                return Module::Sequencer;

            case EventTarget::Diagnostic:
                return Module::Diagnostic;
        }

        return Module::None;
    }

    Message ConvertEvent(
        ModuleId sender,
        const Event& event
    )
    {
        const MessageId messageId =
            static_cast<MessageId>(event.type);

        const Message message
        {
            MessageHeader::Create(
                sender,
                ConvertTarget(event.target),
                NoAttribute,
                messageId
            ),
            static_cast<ObjectId>(event.object),
            static_cast<MessageValue>(event.value)
        };

        return message;
    }
}

namespace EventBus
{
    void Init()
    {
        head = 0;
        tail = 0;
        count = 0;
    }

    bool Publish(
        ModuleId sender,
        const Event& event
    )
    {
        if (count >= EventBusConfig::QueueSize)
        {
            return false;
        }

        if (MessageBus::Count() >= MessageBusConfig::QueueSize)
        {
            return false;
        }

        const Message message =
            ConvertEvent(sender, event);

        if (!MessageBus::Publish(message))
        {
            return false;
        }

        queue[tail] = event;

        tail++;

        if (tail >= EventBusConfig::QueueSize)
        {
            tail = 0;
        }

        count++;

        return true;
    }

    bool Get(Event& event)
    {
        if (count == 0)
        {
            return false;
        }

        event = queue[head];

        head++;

        if (head >= EventBusConfig::QueueSize)
        {
            head = 0;
        }

        count--;

        return true;
    }

    bool HasEvents()
    {
        return count > 0;
    }

    uint8_t Count()
    {
        return count;
    }

    void Clear()
    {
        head = 0;
        tail = 0;
        count = 0;
    }
}
