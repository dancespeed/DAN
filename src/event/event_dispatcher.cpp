#include "event_dispatcher.hpp" 

#include <avr/io.h>

#include "event/event.hpp"
#include "eventbus/eventbus.hpp"
#include "logic/logic.hpp"
#include "pwm/pwm.hpp"

namespace EventDispatcher
{
    void Process()
    {
        Event event;

        if (!EventBus::Get(event))
        {
            return;
        }

        if (event.type ==
            EventType::SystemHeartbeat)
        {
            PORTC ^=
                (1 << PC1);
        }

        Logic::HandleEvent(event);

        PWM::HandleEvent(event);
    }
}