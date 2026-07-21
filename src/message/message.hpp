#pragma once

#include <stdint.h>

// -----------------------------------------------------------------------------
// DAN Internal Message Protocol v1
//
// Message size: 64 bits
//
// Header layout:
//
// 31             24 23             16 15          12 11              0
// ┌────────────────┬─────────────────┬──────────────┬─────────────────┐
// │   Sender ID    │   Receiver ID   │    Flags     │   Message ID    │
// │     8 bits     │      8 bits     │    4 bits    │     12 bits     │
// └────────────────┴─────────────────┴──────────────┴─────────────────┘
//
// Flags layout:
//
// 3             2 1                 0
// ┌───────────────┬──────────────────┐
// │  Attributes   │   Message Type   │
// │    2 bits     │      2 bits      │
// └───────────────┴──────────────────┘
//
// Message body:
//
// Object ID: 16 bits
// Value:     16 bits
// -----------------------------------------------------------------------------

using ModuleId = uint8_t;
using MessageId = uint16_t;
using ObjectId = uint16_t;
using MessageValue = uint16_t;

constexpr ModuleId NoReceiver = 0x00;
constexpr ModuleId Broadcast = 0xFF;

enum class MessageType : uint8_t
{
    Event = 0x00,
    Command = 0x01
};

enum class MessageFlag : uint8_t
{
    None = 0x00,
    System = 0x04,
    Reserved = 0x08
};

struct Message
{
    uint32_t header;
    ObjectId object;
    MessageValue value;
};

namespace MessageHeader
{
    constexpr uint8_t SenderShift = 24;
    constexpr uint8_t ReceiverShift = 16;
    constexpr uint8_t FlagsShift = 12;

    constexpr uint32_t ModuleMask = 0xFF;
    constexpr uint32_t FlagsMask = 0x0F;
    constexpr uint32_t TypeMask = 0x03;
    constexpr uint32_t AttributeMask = 0x0C;
    constexpr uint32_t MessageIdMask = 0x0FFF;

    constexpr uint32_t Create(
        ModuleId sender,
        ModuleId receiver,
        MessageType type,
        MessageFlag flags,
        MessageId messageId)
    {
        return
            (static_cast<uint32_t>(sender) << SenderShift) |
            (static_cast<uint32_t>(receiver) << ReceiverShift) |
            (
                (
                    static_cast<uint32_t>(type) |
                    static_cast<uint32_t>(flags)
                ) & FlagsMask
            ) << FlagsShift |
            (static_cast<uint32_t>(messageId) & MessageIdMask);
    }

    constexpr ModuleId GetSender(
        uint32_t header)
    {
        return static_cast<ModuleId>(
            (header >> SenderShift) & ModuleMask
        );
    }

    constexpr ModuleId GetReceiver(
        uint32_t header)
    {
        return static_cast<ModuleId>(
            (header >> ReceiverShift) & ModuleMask
        );
    }

    constexpr MessageType GetType(
        uint32_t header)
    {
        return static_cast<MessageType>(
            (header >> FlagsShift) & TypeMask
        );
    }

    constexpr uint8_t GetFlags(
        uint32_t header)
    {
        return static_cast<uint8_t>(
            (header >> FlagsShift) & FlagsMask
        );
    }

    constexpr MessageId GetMessageId(
        uint32_t header)
    {
        return static_cast<MessageId>(
            header & MessageIdMask
        );
    }

    constexpr bool HasFlag(
        uint32_t header,
        MessageFlag flag)
    {
        return
            (
                GetFlags(header) &
                static_cast<uint8_t>(flag)
            ) != 0;
    }

    constexpr bool IsValidMessageId(
        MessageId messageId)
    {
        return
            (
                static_cast<uint32_t>(messageId) &
                ~MessageIdMask
            ) == 0;
    }
}

static_assert(
    sizeof(Message) == 8,
    "DAN Message must be exactly 8 bytes"
);