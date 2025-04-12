#include "Distortion.h"

Distortion::Distortion()
{
    // Set a default threshold (e.g., -6.0 dB) for the distortion effect.
    thresholdDecibels = -6.0f;
    negativeThresholdGain = -juce::Decibels::decibelsToGain(thresholdDecibels);
    positiveThresholdGain = juce::Decibels::decibelsToGain(thresholdDecibels);
}

Distortion::~Distortion()
{
    // Cleanup if needed.
}

void Distortion::setThresholdDecibels(float newThreshold)
{
    thresholdDecibels = newThreshold;
    negativeThresholdGain = -juce::Decibels::decibelsToGain(thresholdDecibels);
    positiveThresholdGain = juce::Decibels::decibelsToGain(thresholdDecibels);
}

float Distortion::getThresholdDecibels() const
{
    return thresholdDecibels;
}

void Distortion::process(juce::AudioBuffer<float>& buffer)
{
    // Apply hard clipping to each channel in the audio buffer.
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        juce::FloatVectorOperations::clip(
            channelData,                // destination pointer (in-place)
            channelData,                // source pointer
            negativeThresholdGain,      // lower bound
            positiveThresholdGain,      // upper bound
            buffer.getNumSamples()      // number of samples to process
        );
    }
}
