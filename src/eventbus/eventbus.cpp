#include "eventbus.hpp"
#include "eventbus_config.hpp"

namespace
{
    Event queue[EventBusConfig::QueueSize];

    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t count = 0;
}

namespace EventBus
{
    void Init()
    {
        head = 0;
        tail = 0;
        count = 0;
    }

    bool Publish(const Event& event)
    {
        if (count >= EventBusConfig::QueueSize)
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