#include "system.hpp"
#include "system_config.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

namespace
{
    volatile uint32_t tickMs = 0;
    uint32_t lastHeartbeatMs = 0;
}

ISR(TIMER2_COMPA_vect)
{
    tickMs++;
}

namespace System
{
    void Init()
    {
        // PC1 as output
        DDRC |= (1 << SystemConfig::HeartbeatBit);

        // Heartbeat LOW initially
        PORTC &= ~(1 << SystemConfig::HeartbeatBit);

        // Timer1 CTC mode, 1 mc tick
        TCCR2A = 0;
        TCCR2B = 0;
        TCNT2 = 0;

        OCR2A = SystemConfig::Timer1CompareValue;
        // OCR2A = 249;

        TCCR2A |= (1 << WGM21);                 // CTC mode
        TCCR2B |= (1 << CS22) | (1 << CS22);    // prescaler 64

        TIMSK2 |= (1 << OCIE2A);                // enable compare interrupt

        sei();
    }

    uint32_t GetTickMs()
    {
        uint32_t value;

        uint8_t sreg = SREG;
        cli();
        
        value = tickMs;
        
        SREG = sreg;

        return value;
    }
}