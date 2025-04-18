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

    /**
     * Call this from your processor’s prepareToPlay()
     * to give AutoTune the sample rate, maximum block size, and channel count.
     */
    void prepare(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 numChannels);

    void setDryWetMix(float mix);
    float getDryWetMix() const;

    /** Process one buffer of audio in your processBlock(). */
    void process(juce::AudioBuffer<float>& buffer);

private:
    float dryWetMix;

    // Stored from prepare()
    double        currentSampleRate{ 44100.0 };
    juce::uint32  currentMaxBlockSize{ 512 };
    juce::uint32  currentNumChannels{ 2 };

    std::vector<float> validFrequencies;

    void generateNotesFromKey(const juce::String& keySignature);
    float detectPitch(const float* channelData, int numSamples, double sampleRate);
    float findClosestFrequency(float detectedPitch);
    float calculateShiftFactor(float detectedPitch, float targetFrequency);
    void pitchShiftBuffer(juce::AudioBuffer<float>& buffer, float shiftFactor);
};
