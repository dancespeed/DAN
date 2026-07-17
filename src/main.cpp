#include <Arduino.h>

#include "system/system.hpp"
#include "event/event.hpp"
#include "eventbus/eventbus.hpp"
#include "logic/logic.hpp"
#include "drivers/pwm_driver.hpp"
#include "pwm/pwm.hpp"
#include "pwm/pwm_types.hpp"

namespace
{
    constexpr uint32_t HeartbeatPeriodMs = 1000;
    constexpr uint32_t CommandPeriodMs = 5000;

    constexpr ChannelId TestChannel =
        ChannelId::Channel1;

    constexpr CommandType TestCommands[] =
    {
        CommandType::Click,
        CommandType::Click,
        CommandType::Click,
        CommandType::Click,
        CommandType::Hold
    };

    constexpr uint8_t TestCommandCount =
        sizeof(TestCommands) /
        sizeof(TestCommands[0]);

    bool systemReady = false;
}

void setup()
{
    Serial.begin(115200);

    System::Init();
    EventBus::Init();
    Logic::Init();

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

    Serial.println(
        "Logic command test started"
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

    static uint8_t commandIndex = 0;

    const uint32_t now =
        System::GetTickMs();

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

        EventBus::Publish(
            heartbeatEvent
        );
    }

    if ((now - lastCommandTick) >=
        CommandPeriodMs)
    {
        lastCommandTick = now;

        const CommandType command =
            TestCommands[commandIndex];

        const Event commandEvent
        {
            EventType::UserCommand,
            EventTarget::Logic,
            static_cast<uint8_t>(
                TestChannel
            ),
            static_cast<uint16_t>(
                command
            )
        };

        EventBus::Publish(
            commandEvent
        );

        Serial.print(
            "Command: "
        );

        if (command ==
            CommandType::Click)
        {
            Serial.println(
                "Click"
            );
        }
        else if (command ==
                 CommandType::Hold)
        {
            Serial.println(
                "Hold"
            );
        }

        commandIndex++;

        if (commandIndex >=
            TestCommandCount)
        {
            commandIndex = 0;

            Serial.println(
                "Command sequence restarted"
            );
        }
    }

    Event event;

    if (EventBus::Get(event))
    {
        if (event.type ==
            EventType::SystemHeartbeat)
        {
            PORTC ^=
                (1 << PC1);
        }

        Logic::HandleEvent(event);
        PWM::HandleEvent(event);

        if (event.type ==
            EventType::ChannelStateChanged)
        {
            Serial.print(
                "Channel brightness: "
            );

            Serial.println(
                event.value
            );
        }
    }

    PWM::Process();
}