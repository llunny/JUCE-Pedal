// Overdrive.cpp
#include "Overdrive.h"
#include <cmath>

Overdrive::Overdrive(float baseDrive_, float maxDrive_, float energySens_, float dryWetMix_)
    : baseDrive(baseDrive_), maxDrive(maxDrive_), energySensitivity(energySens_), dryWetMix(dryWetMix_)
{
}

Overdrive::~Overdrive() {}

void Overdrive::setDryWetMix(float mix) { dryWetMix = mix; }
float Overdrive::getDryWetMix() const { return dryWetMix; }

float Overdrive::computeRMS(const float* data, int numSamples)
{
    double sum = 0.0;
    for (int i = 0; i < numSamples; ++i)
        sum += data[i] * data[i];
    return std::sqrt(sum / numSamples);
}

void Overdrive::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer(channel);
        float energy = computeRMS(data, numSamples);
        float drive = baseDrive + energy * energySensitivity;
        if (drive > maxDrive) drive = maxDrive;

        for (int i = 0; i < numSamples; ++i)
        {
            float x = data[i];
            float sat = std::tanh(drive * x);
            data[i] = dryWetMix * sat + (1.0f - dryWetMix) * x;
        }
    }
}