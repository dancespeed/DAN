#include <Arduino.h>

#include "system/system.hpp"
#include "event/event.hpp"
#include "eventbus/eventbus.hpp"

void setup()
{
    System::Init();
    EventBus::Init();
}

void loop()
{

  static uint32_t lastTick = 0;

  const uint32_t now = System::GetTickMs();

  if ((now - lastTick) >= 1000)
  {
    lastTick = now;

    Event event
    {
      EventType::SystemHeartbeat,
      EventTarget::Broadcast,
      0,
      0
    };

    EventBus::Publish(event);
  }

  Event event;

  if (EventBus::Get(event))
  {
    if (event.type == EventType::SystemHeartbeat)
    {
      PORTC ^= (1 << PC1);
    }
  }
}