#include "input.hpp"

#include "input_config.hpp"
#include "pcf8574_input.hpp"

#include "event/event.hpp"
#include "eventbus/eventbus.hpp"

#include "message/module_id.hpp"

#include "pwm/pwm_types.hpp"

#include "system/system.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

namespace
{
    volatile bool pcfInterruptPending = false;

    ButtonRuntime buttons[
        static_cast<uint8_t>(ButtonId::Count)
    ];

    uint8_t ToIndex(ButtonId button)
    {
        return static_cast<uint8_t>(button);
    }

    bool IsValidButton(ButtonId button)
    {
        const uint8_t index =
            ToIndex(button);

        return
            (button != ButtonId::None) &&
            (index <
             static_cast<uint8_t>(
                 ButtonId::Count
             ));
    }

    bool TryGetChannel(ButtonId button, ChannelId& channel)
    {
        switch (button)
        {
            case ButtonId::SturmLh:
            {
                channel = ChannelId::SturmLh;
                return true;
            }

            case ButtonId::SturmRh:
            {
                channel = ChannelId::SturmRh;
                return true;
            }

            case ButtonId::Side:
            {
                channel = ChannelId::Side;
                return true;
            }

            case ButtonId::Res1:
            {
                channel = ChannelId::Res1;
                return true;
            }

            case ButtonId::Res2:
            {
                channel = ChannelId::Res2;
                return true;
            }

            case ButtonId::Smart:
            {
                channel = ChannelId::Smart;
                return true;
            }

            case ButtonId::Exhst:
            {
                channel = ChannelId::Exhst;
                return true;
            }

            case ButtonId::Ceil:
            {
                channel = ChannelId::Ceil;
                return true;
            }

            case ButtonId::AromaIon:
            {
                // Специальная кнопка, управляющая
                // двумя каналами. Реализацию добавим
                // отдельным кирпичом.
                return false;
            }

            case ButtonId::None:
            case ButtonId::Count:
            default:
            {
                return false;
            }
        }
    }

    void PublishCommand(ButtonId button, CommandType command)
    {
        ChannelId channel;
        CommandType outputCommand = command;

        if (button == ButtonId::AromaIon)
        {
            if (command == CommandType::Click)
            {
                channel = ChannelId::Aroma;
            }
            else if (command == CommandType::Hold)
            {
                channel = ChannelId::Ion;

                // Физический Hold этой кнопки
                // означает переключение канала ION.
                outputCommand = CommandType::Click;
            }
            else
            {
                return;
            }
        }
        else
        {
            if (!TryGetChannel(button, channel))
            {
                return;
            }
        }

        const Event event
        {
            EventType::UserCommand,
            EventTarget::Logic,
            static_cast<uint8_t>(channel),
            static_cast<uint16_t>(outputCommand)
        };

        EventBus::Publish(Module::Button, event);
    }

    void SetRawState(
        ButtonId button,
        bool pressed,
        uint32_t now
    )
    {
        if (!IsValidButton(button))
        {
            return;
        }

        ButtonRuntime& state =
            buttons[ToIndex(button)];

        if (pressed == state.rawPressed)
        {
            return;
        }

        state.rawPressed = pressed;
        state.rawChangedMs = now;
    }

    void UpdatePcfRawStates(
        uint8_t portValue,
        uint32_t now
    )
    {
        // Нажатая кнопка формирует LOW.
        SetRawState(
            ButtonId::SturmLh,
            (portValue & (1 << 0)) == 0,
            now
        );

        SetRawState(
            ButtonId::SturmRh,
            (portValue & (1 << 1)) == 0,
            now
        );

        SetRawState(
            ButtonId::Side,
            (portValue & (1 << 2)) == 0,
            now
        );

        SetRawState(
            ButtonId::Res1,
            (portValue & (1 << 3)) == 0,
            now
        );

        SetRawState(
            ButtonId::Res2,
            (portValue & (1 << 4)) == 0,
            now
        );

        SetRawState(
            ButtonId::AromaIon,
            (portValue & (1 << 5)) == 0,
            now
        );

        SetRawState(
            ButtonId::Smart,
            (portValue & (1 << 6)) == 0,
            now
        );

        SetRawState(
            ButtonId::Exhst,
            (portValue & (1 << 7)) == 0,
            now
        );
    }

    void UpdateCeilRawState(uint32_t now)
    {
        const bool pressed =
            (PINC &
             (1 << InputConfig::CeilButtonBit))
            == 0;

        SetRawState(
            ButtonId::Ceil,
            pressed,
            now
        );
    }

    void ProcessButton(
        ButtonId button,
        uint32_t now
    )
    {
        ButtonRuntime& state =
            buttons[ToIndex(button)];

        // Сырое состояние должно оставаться неизменным
        // не менее DebounceTimeMs.
        if (state.rawPressed !=
            state.stablePressed)
        {
            if ((now - state.rawChangedMs) <
                InputConfig::DebounceTimeMs)
            {
                return;
            }

            state.stablePressed =
                state.rawPressed;

            if (state.stablePressed)
            {
                // Подтверждённое нажатие.
                state.pressedMs = now;
                state.holdPublished = false;
            }
            else
            {
                // Подтверждённое отпускание.
                const uint32_t pressDuration =
                    now - state.pressedMs;

                if (!state.holdPublished &&
                    pressDuration <=
                        InputConfig::ClickMaxTimeMs)
                {
                    PublishCommand(
                        button,
                        CommandType::Click
                    );
                }

                // Для диапазона 1500...2000 мс
                // команда не публикуется.
                //
                // После Hold отпускание также
                // не публикует Click.
                state.holdPublished = false;
            }
        }

        if (!state.stablePressed)
        {
            return;
        }

        if (state.holdPublished)
        {
            return;
        }

        if ((now - state.pressedMs) >=
            InputConfig::HoldTimeMs)
        {
            state.holdPublished = true;

            PublishCommand(
                button,
                CommandType::Hold
            );
        }
    }

    void ProcessAllButtons(uint32_t now)
    {
        for (
            uint8_t i =
                static_cast<uint8_t>(
                    ButtonId::None
                ) + 1;

            i <
                static_cast<uint8_t>(
                    ButtonId::Count
                );

            i++
        )
        {
            ProcessButton(
                static_cast<ButtonId>(i),
                now
            );
        }
    }
}

ISR(INT0_vect)
{
    // В ISR не используем I2C
    // и не публикуем события.
    pcfInterruptPending = true;
}

namespace Input
{
    bool Init()
    {
        const uint32_t now =
            System::GetTickMs();

        for (
            uint8_t i = 0;
            i <
                static_cast<uint8_t>(
                    ButtonId::Count
                );
            i++
        )
        {
            buttons[i].rawPressed = false;
            buttons[i].stablePressed = false;

            buttons[i].holdPublished = false;

            buttons[i].rawChangedMs = now;
            buttons[i].pressedMs = now;
        }

        // CEIL / PC0 — вход.
        DDRC &=
            ~(1 << InputConfig::CeilButtonBit);

        // Внешняя подтяжка 10 кОм уже есть.
        PORTC &=
            ~(1 << InputConfig::CeilButtonBit);

        // PCF8574 INT / PD2 — вход.
        DDRD &=
            ~(1 << InputConfig::PcfInterruptBit);

        // Дополнительная внутренняя подтяжка INT0.
        PORTD |=
            (1 << InputConfig::PcfInterruptBit);

        if (!PCF8574Input::Init())
        {
            return false;
        }

        uint8_t pcfValue = 0xFF;

        if (!PCF8574Input::Read(pcfValue))
        {
            return false;
        }

        // Начальное состояние PCF8574.
        UpdatePcfRawStates(
            pcfValue,
            now
        );

        // Начальное состояние CEIL.
        UpdateCeilRawState(now);

        // При включении уже зажатые кнопки
        // не должны создавать Click или Hold.
        for (
            uint8_t i =
                static_cast<uint8_t>(
                    ButtonId::None
                ) + 1;

            i <
                static_cast<uint8_t>(
                    ButtonId::Count
                );

            i++
        )
        {
            ButtonRuntime& state =
                buttons[i];

            state.stablePressed =
                state.rawPressed;

            state.holdPublished =
                state.rawPressed;
        }

        pcfInterruptPending = false;

        // INT0 по спадающему фронту.
        EICRA &=
            ~(1 << ISC00);

        EICRA |=
            (1 << ISC01);

        // Сбрасываем возможный старый флаг.
        EIFR |=
            (1 << INTF0);

        // Разрешаем INT0.
        EIMSK |=
            (1 << INT0);

        return true;
    }

    void Process()
    {
        const uint32_t now =
            System::GetTickMs();

        // CEIL читается непосредственно
        // в каждом проходе.
        UpdateCeilRawState(now);

        bool readPcf = false;

        {
            const uint8_t sreg = SREG;

            cli();

            if (pcfInterruptPending)
            {
                pcfInterruptPending = false;
                readPcf = true;
            }

            SREG = sreg;
        }

        if (readPcf)
        {
            uint8_t pcfValue = 0xFF;

            if (PCF8574Input::Read(pcfValue))
            {
                UpdatePcfRawStates(
                    pcfValue,
                    now
                );
            }
        }

        ProcessAllButtons(now);
    }

    bool IsPressed(ButtonId button)
    {
        if (!IsValidButton(button))
        {
            return false;
        }

        return buttons[
            ToIndex(button)
        ].stablePressed;
    }
}