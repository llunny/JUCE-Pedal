// AutoTune.h
#pragma once
#include <JuceHeader.h>
#include <vector>

/**
 * AutoTune
 * Implements real-time pitch correction based on a specified key signature.
 */
class AutoTune
{
public:
    AutoTune(const juce::String& keySignature, float dryWetMix = 1.0f);
    ~AutoTune();

    void setDryWetMix(float mix);
    float getDryWetMix() const;

    void process(juce::AudioBuffer<float>& buffer);

private:
    float dryWetMix;
    std::vector<float> validFrequencies;

    void generateNotesFromKey(const juce::String& keySignature);
    float detectPitch(const float* channelData, int numSamples, double sampleRate);
    float findClosestFrequency(float detectedPitch);
    float calculateShiftFactor(float detectedPitch, float targetFrequency);
    void pitchShiftBuffer(juce::AudioBuffer<float>& buffer, float shiftFactor);
};