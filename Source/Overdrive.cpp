// Harmony.h
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <juce_dsp/juce_dsp.h>   // or your custom PitchShifter shim

#include "PitchShifter.h"


class Harmony
{
public:
    Harmony(const juce::String& keySignature, float dryWetMix = 0.5f);
    ~Harmony() = default;

    /** Call from prepareToPlay() */
    void prepare(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 numChannels);

    void setDryWetMix(float mix);
    float getDryWetMix() const;

    /** Call from processBlock() */
    void process(juce::AudioBuffer<float>& buffer);

private:
    float dryWetMix;

    double       currentSampleRate{ 44100.0 };
    juce::uint32 currentMaxBlockSize{ 512 };
    juce::uint32 currentNumChannels{ 2 };

    std::vector<float>             scaleFrequencies;
    juce::dsp::PitchShifter<float> pitchShifter;

    void generateNotesFromKey(const juce::String& keySignature);
    float detectPitch(const float* channelData, int numSamples, double sampleRate);
};
