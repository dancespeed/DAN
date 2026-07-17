#include <Arduino.h>

#include "system/system.hpp"
#include "event/event.hpp"
#include "eventbus/eventbus.hpp"

#include "drivers/pwm_driver.hpp"
#include "pwm/pwm.hpp"

namespace
{
    constexpr uint32_t HeartbeatPeriodMs = 1000;

    bool systemReady = false;
}

void setup()
{
    Serial.begin(115200);

    System::Init();
    EventBus::Init();

    if (!PWMDriver::Init())
    {
        Serial.println(
            "PWM driver initialization failed"
        );

        return;
    }

    PWM::Init();

    systemReady = true;

    Serial.println(
        "DAN Platform initialized"
    );
}

void loop()
{
    if (!systemReady)
    {
        return;
    }

    static uint32_t lastHeartbeatTick = 0;

    const uint32_t now = System::GetTickMs();

    if ((now - lastHeartbeatTick) >=
        HeartbeatPeriodMs)
    {
        lastHeartbeatTick = now;

        const Event heartbeatEvent
        {
            EventType::SystemHeartbeat,
            EventTarget::Broadcast,
            0,
            0
        };

        EventBus::Publish(heartbeatEvent);
    }

    Event event;

    if (EventBus::Get(event))
    {
        if (event.type ==
            EventType::SystemHeartbeat)
        {
            PORTC ^= (1 << PC1);
        }

        PWM::HandleEvent(event);
    }

    PWM::Process();
}