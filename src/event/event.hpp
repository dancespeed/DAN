#pragma once

#include <stdint.h>

enum class EventType : uint8_t
{
    None = 0,

    SystemHeartbeat,

    IgnitionOn,
    IgnitionOff,

    UserCommand,

    SetChannelState,
    ChannelStateChanged,

    StartSequence,
    SequenceFinished,

    RequestEepromFlush,
    RequestSleep
};

enum class EventTarget : uint8_t
{
    Broadcast = 0,

    System,
    Button,
    PWM,
    EEPROM,
    CAN,
    Logic,
    Sequencer,
    Diagnostic
};

enum class CommandType : uint8_t
{
    None = 0,

    Click,
    Hold
};

enum class SequenceId : uint8_t
{
    None = 0,

    Startup,
    Shutdown
};

struct Event
{
    EventType type;
    EventTarget target;
    uint8_t object;
    uint16_t value;
};