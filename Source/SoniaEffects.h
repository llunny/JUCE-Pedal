/*
  ==============================================================================

    SoniaEffects.h
    Created: 16 Mar 2025 5:30:46pm
    Author:  wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 * SoniaEffects
 * -------------------------
 * A placeholder class for Sonia’s audio effects.
 * Sonia can add her own methods, parameters, and data structures
 * needed for real-time DSP (e.g., reverb, distortion, pitch shifting, etc.).
 */
class SoniaEffects
{
public:
    SoniaEffects();
    ~SoniaEffects();

    // Called every audio block to process the signal.
    void process(juce::AudioBuffer<float>& buffer);

private:
    // Example placeholder variables or DSP structures go here.
    // For instance:
    // float reverbAmount = 0.5f;
    // float driveGain = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoniaEffects)
};
