#include <Arduino.h>

#include "system/system.hpp"
#include "event/event.hpp"
#include "eventbus/eventbus.hpp"
#include "pwm/pwm.hpp"

void setup()
{
    Serial.begin(115200);

    System::Init();
    EventBus::Init();
    PWM::Init();

    Serial.println("DAN PWM transition test started");
}

void loop()
{
    static uint32_t lastHeartbeatTick = 0;
    static uint32_t lastCommandTick = 0;
    static uint32_t lastPrintTick = 0;

    static uint8_t testStep = 1;

    const uint32_t now = System::GetTickMs();

    // Существующий тест EventBus и светодиода PC1.
    if ((now - lastHeartbeatTick) >= 1000)
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

    // Команда тестовому PWM-каналу каждые пять секунд.
    if ((now - lastCommandTick) >= 5000)
    {
        lastCommandTick = now;

        const Event pwmEvent
        {
            EventType::SetChannelState,
            EventTarget::PWM,
            static_cast<uint8_t>(ChannelId::Channel1),
            testStep
        };

        EventBus::Publish(pwmEvent);

        Serial.print("Command: step = ");
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
        if (event.type == EventType::SystemHeartbeat)
        {
            PORTC ^= (1 << PC1);
        }

        PWM::HandleEvent(event);

        if (event.type == EventType::ChannelStateChanged)
        {
            Serial.print("Transition finished, brightness = ");
            Serial.println(event.value);
        }
    }

    PWM::Process();

    // Наблюдение за переходом в RAM.
    if ((now - lastPrintTick) >= 250)
    {
        lastPrintTick = now;

        Serial.print("Step: ");
        Serial.print(PWM::GetCurrentStep(ChannelId::Channel1));

        Serial.print(" | Enabled: ");
        Serial.print(PWM::IsEnabled(ChannelId::Channel1));

        Serial.print(" | Brightness: ");
        Serial.println(
            PWM::GetCurrentBrightness(ChannelId::Channel1)
        );
    }
}