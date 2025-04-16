// Reverb.cpp
#include "Reverb.h"
#include <cmath>

Reverb::Reverb(int bufLen, float normDec, float fastDec, float noteThresh, float mix)
    : bufferLength(bufLen), normalDecay(normDec), fastDecay(fastDec), noteOnThreshold(noteThresh), reverbMix(mix)
{
    reverbBuffer.assign(bufferLength, 0.0f);
    bufferIndex = 0;
}

Reverb::~Reverb() {}

void Reverb::setReverbMix(float mix) { reverbMix = mix; }
float Reverb::getReverbMix() const { return reverbMix; }

float Reverb::computeRMS(const float* data, int numSamples)
{
    double sum = 0.0;
    for (int i = 0; i < numSamples; ++i)
        sum += data[i] * data[i];
    return std::sqrt(sum / numSamples);
}

void Reverb::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    // Compute block energy on first channel
    float blockEnergy = computeRMS(buffer.getReadPointer(0), numSamples);
    float decay = (blockEnergy > noteOnThreshold) ? fastDecay : normalDecay;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer(channel);
        for (int i = 0; i < numSamples; ++i)
        {
            float x = data[i];
            float delayed = reverbBuffer[bufferIndex];
            float newRev = x + delayed * decay;
            reverbBuffer[bufferIndex] = newRev;

            data[i] = (1.0f - reverbMix) * x + reverbMix * newRev;
            bufferIndex = (bufferIndex + 1) % bufferLength;
        }
    }
}