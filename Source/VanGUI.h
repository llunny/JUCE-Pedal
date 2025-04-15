/*
  ==============================================================================

    VanGUI.h
    Created: 16 Mar 2025 5:30:23pm
    Author:  wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <vector>

/**
 * VanGUI
 * -------------------------
 * A placeholder class for Van’s GUI components and layout.
 * Van can add knobs, sliders, or advanced visualization here.
 */

class SmartPedalAudioProcessor;
 


class VanGUI : public juce::Component, private juce::Timer
{
public:
    VanGUI();
    ~VanGUI() override;

    // Called after construction to set up child components (sliders, buttons, etc.)
    void initialize();

    // Optionally, call this periodically if you want to update the GUI
    void update();

    // juce::Component overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Example placeholders for GUI controls:
    // juce::Slider exampleSlider;
    // juce::TextButton loadModelButton;

    SmartPedalAudioProcessor& processor;
    void timerCallback() override;


    juce::AudioBuffer<float> fakeBuffer;
    juce::int64 lastUpdateTime = 0;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VanGUI)
};

