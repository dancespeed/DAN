#include <Arduino.h>

#include "system/system.hpp"
#include "event/event.hpp"
#include "eventbus/eventbus.hpp"

#include "drivers/pwm_driver.hpp"
#include "pwm/pwm.hpp"
#include "pwm/pwm_types.hpp"

namespace
{
    constexpr uint32_t HeartbeatPeriodMs = 1000;
    constexpr uint32_t CommandPeriodMs = 5000;

    constexpr ChannelId TestChannel =
        ChannelId::Channel1;

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
        "Real PWM transition test started"
    );
}

void loop()
{
    if (!systemReady)
    {
        return;
    }

    static uint32_t lastHeartbeatTick = 0;
    static uint32_t lastCommandTick = 0;

    static uint8_t testStep = 1;

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

    if ((now - lastCommandTick) >=
        CommandPeriodMs)
    {
        lastCommandTick = now;

        const Event pwmEvent
        {
            EventType::SetChannelState,
            EventTarget::PWM,
            static_cast<uint8_t>(TestChannel),
            testStep
        };

        EventBus::Publish(pwmEvent);

        Serial.print("Command step: ");
        Serial.println(testStep);

        if (testStep < 4)
        {
            testStep++;
        }
        else if (testStep == 4)
        {
            testStep = 0;
        }
        else
        {
            testStep = 1;
        }
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

        if (event.type ==
            EventType::ChannelStateChanged)
        {
            Serial.print(
                "Transition finished. Brightness: "
            );

            Serial.println(event.value);
        }
    }

    PWM::Process();
}