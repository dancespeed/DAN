#include "sequencer.hpp"

namespace
{
    const Sequencer::SequenceStep* activeSequence = nullptr;
    uint8_t activeSequenceSize = 0;
    uint8_t currentStep = 0;
}

namespace Sequencer
{
    void Init()
    {
        activeSequence = nullptr;
        activeSequenceSize = 0;
        currentStep = 0;
    }

    void Load(const SequenceStep* sequence, uint8_t size)
    {
        activeSequence = sequence;
        activeSequenceSize = size;
        currentStep = 0;
    }

    void HandleEvent(const Event& event)
    {
        (void)event;
    }

    void Update()
    {
    }
}