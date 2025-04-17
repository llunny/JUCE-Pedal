// Harmony.h
#pragma once
#include <JuceHeader.h>
#include <vector>
#include <juce_dsp/juce_dsp.h>

/**
 * Harmony
 * Generates 3rd and 5th harmonies based on detected pitch and mixes them with the dry signal.
 */
class Harmony
{
public:
    Harmony(const juce::String& keySignature, float dryWetMix = 0.5f);
    ~Harmony();

    void setDryWetMix(float mix);
    float getDryWetMix() const;

    void process(juce::AudioBuffer<float>& buffer);

private:
    float dryWetMix;
    std::vector<float> scaleFrequencies;
    juce::dsp::PitchShifter<float> pitchShifter;

    void generateNotesFromKey(const juce::String& keySignature);
    float detectPitch(const float* channelData, int numSamples, double sampleRate);
};