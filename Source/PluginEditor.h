/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "VanGUI.h"

//==============================================================================
/**
*/
class SmartPedalAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    
    
    
    SmartPedalAudioProcessorEditor(SmartPedalAudioProcessor&);
    ~SmartPedalAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    
    
    
    
    void buttonClicked(juce::Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SmartPedalAudioProcessor& audioProcessor;
    
    
    // adds Distortion Button
    juce::TextButton Distortion;
    
    juce::TextButton Reverberation;
    juce::TextButton Overdrive;
    juce::TextButton Harmony;

    VanGUI vanGUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmartPedalAudioProcessorEditor)
};
