// Reverb.h
#pragma once
#include <JuceHeader.h>
#include <vector>

/**
 * Reverb
 * Implements a simple feedback delay network with adaptive decay based on note onsets.
 */
class Reverb
{
public:
    Reverb(int bufferLength = 48000,
                float normalDecay = 0.98f,
                float fastDecay = 0.5f,
                float noteOnThreshold = 0.1f,
                float reverbMix = 0.4f);
    ~Reverb();

    void setReverbMix(float mix);
    float getReverbMix() const;

    void process(juce::AudioBuffer<float>& buffer);

private:
    float normalDecay, fastDecay, noteOnThreshold, reverbMix;
    int bufferLength;
    std::vector<float> reverbBuffer;
    int bufferIndex;

    float computeRMS(const float* data, int numSamples);
};