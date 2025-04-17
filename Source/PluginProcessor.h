/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SoniaEffects.h"
#include "ScopeDataCollector.h"
#include "Distortion.h"

#include "Overdrive.h"
#include "Harmony.h"
#include "Reverb.h"



//==============================================================================
/**
*/
class SmartPedalAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SmartPedalAudioProcessor();
    ~SmartPedalAudioProcessor() override;

    const juce::AudioBuffer<float>& getScopeBuffer();
    
    
    void setDistortionEnabled(bool enabled);
    bool isDistortionEnabled() const { return distortionEnabled; }
    


    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    ScopeDataCollector scopeCollector;

private:
    //==============================================================================

    // An instance of Sonia's placeholder effects class
//    SoniaEffects soniaEffects;
    Distortion distortion;
    Overdrive overdrive;
    Harmony harmony;
    Reverb reverb;
    
    juce::AudioBuffer<float> scopeBuffer;
    juce::AudioBuffer<float> scopeSnapshot;
    
    bool distortionEnabled = false;


    std::mutex scopeMutex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmartPedalAudioProcessor)
};
