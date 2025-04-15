/*
  ==============================================================================

    ScopeDataCollector.h
    Created: 15 Apr 2025 2:49:57pm
    Author:  wllun

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <array>

class ScopeDataCollector
{
public:
    ScopeDataCollector() : fifoIndex(0) {}

    void processBlock(const juce::AudioBuffer<float>& buffer)
    {
        const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();

        for (int i = 0; i < numSamples; ++i)
        {
            float sample = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch)
                sample += buffer.getReadPointer(ch)[i];

            sample /= static_cast<float>(numChannels); // average to mono
            pushSample(sample);
        }
    }

    void pushSample(float sample)
    {
        if (fifoIndex >= bufferSize)
            fifoIndex = 0;

        fifo[fifoIndex++] = sample;
    }

    const std::array<float, 1024>& getFifo() const { return fifo; }

private:
    static constexpr int bufferSize = 1024;
    std::array<float, 1024> fifo;
    int fifoIndex;
};


