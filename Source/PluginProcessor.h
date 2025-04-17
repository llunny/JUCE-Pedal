
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
#include "Autotune.h"



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


    void setReverberationEnabled(bool enabled_1);
    bool isReverberationEnabled() const { return ReverberationEnabled; }


    void setHarmonyEnabled(bool enabled_2);
    bool isHarmonyEnabled() const {return HarmonyEnabled; }

    void setOverdriveEnabled(bool enabled_3);
    bool isOverdriveEnabled() const { return OverdriveEnabled; }


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
    Autotune autotune;
    Overdrive overdrive;
    Harmony harmony;
    Reverb reverb;
    
    juce::AudioBuffer<float> scopeBuffer;
    juce::AudioBuffer<float> scopeSnapshot;
    
    bool distortionEnabled = false;
    bool ReverberationEnabled = false;
    bool HarmonyEnabled = false;
    bool OverdriveEnabled = false;
    




    std::mutex scopeMutex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmartPedalAudioProcessor)
};
