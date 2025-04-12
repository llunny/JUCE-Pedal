#pragma once

#include <JuceHeader.h>

/**
 * Distortion
 * -------------------------
 * A class implementing a simple hard-clipping distortion effect.
 * It applies hard clipping to audio samples based on a threshold in decibels.
 */
class Distortion
{
public:
    Distortion();
    ~Distortion();

    // Process the audio buffer and apply distortion.
    void process(juce::AudioBuffer<float>& buffer);

    // Set the clipping threshold in decibels.
    void setThresholdDecibels(float newThreshold);

    // Get the current clipping threshold in decibels.
    float getThresholdDecibels() const;

private:
    // Distortion threshold in decibels.
    float thresholdDecibels;

    // Calculated gain values for clipping.
    float negativeThresholdGain;
    float positiveThresholdGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Distortion)
};
