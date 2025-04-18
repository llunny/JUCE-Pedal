// Overdrive.h
#pragma once
#include <JuceHeader.h>

/**
 * Overdrive
 * Applies a non-linear tanh-based saturation whose intensity adapts to block energy.
 */
class Overdrive
{
public:
    Overdrive(float baseDrive = 1.0f,
        float maxDrive = 10.0f,
        float energySensitivity = 5.0f,
        float dryWetMix = 0.7f);
    ~Overdrive();

    void setDryWetMix(float mix);
    float getDryWetMix() const;

    void process(juce::AudioBuffer<float>& buffer);

private:
    float baseDrive, maxDrive, energySensitivity, dryWetMix;

    float computeRMS(const float* data, int numSamples);
};
