/*
  ==============================================================================

    VanGUI.cpp
    Created: 16 Mar 2025 5:30:07pm
    Author:  wllun

  ==============================================================================
*/

#include "VanGUI.h"

VanGUI::VanGUI(SmartPedalAudioProcessor& p) : processor(p)
{

    // Constructor: You can create child components here if you like.
    startTimerHz(60);
}

VanGUI::~VanGUI()
{
    // Destructor: Clean up if needed.
}

void VanGUI::initialize()
{
    // This method is called to set up and add child components to the GUI.
    // For example:
    // addAndMakeVisible(exampleSlider);
    // exampleSlider.setRange(0.0, 100.0);
    // ...
}

void VanGUI::update()
{
    // If you need to update the GUI in real-time (e.g., refresh waveforms or meters),
    // you can do it here. This could be called by a timer or from PluginEditor.
}

void VanGUI::paint(juce::Graphics& g)
{
  
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::lime);

    const auto& buffer = processor.getScopeBuffer();
    const float* samples = buffer.getReadPointer(0);
    const int numSamples = buffer.getNumSamples();

    const auto width = getWidth();
    const auto height = getHeight();

    juce::Path waveform;
    waveform.startNewSubPath(0, height / 2);

    for (int i = 0; i < numSamples; ++i)
    {
        float x = static_cast<float>(i) / numSamples * width;
        float y = juce::jmap(samples[i], -1.0f, 1.0f, static_cast<float>(height), 0.0f);
        waveform.lineTo(x, y);
    }

    g.strokePath(waveform, juce::PathStrokeType(2.0f));

  
}

void VanGUI::resized()
{
    // Position your child components here, e.g.:
    // exampleSlider.setBounds (10, 10, getWidth() - 20, 40);
}

/*
  ==============================================================================

    VanGUI.cpp
    Created: 16 Mar 2025 5:30:07pm
    Author:  wllun

  ==============================================================================
*/

#include "VanGUI.h"

VanGUI::VanGUI(SmartPedalAudioProcessor& p) : processor(p)
{
    // Constructor: You can create child components here if you like.
    startTimerHz(60);
}

VanGUI::~VanGUI()
{
    // Destructor: Clean up if needed.
}

void VanGUI::initialize()
{
    // This method is called to set up and add child components to the GUI.
    // For example:
    // addAndMakeVisible(exampleSlider);
    // exampleSlider.setRange(0.0, 100.0);
    // ...
}

void VanGUI::update()
{
    // If you need to update the GUI in real-time (e.g., refresh waveforms or meters),
    // you can do it here. This could be called by a timer or from PluginEditor.
}

void VanGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::lime);

    const auto& buffer = processor.getScopeBuffer();
    const float* samples = buffer.getReadPointer(0);
    const int numSamples = buffer.getNumSamples();

    const auto width = getWidth();
    const auto height = getHeight();

    juce::Path waveform;
    waveform.startNewSubPath(0, height / 2);

    for (int i = 0; i < numSamples; ++i)
    {
        float x = static_cast<float>(i) / numSamples * width;
        float y = juce::jmap(samples[i], -1.0f, 1.0f, static_cast<float>(height), 0.0f);
        waveform.lineTo(x, y);
    }

    g.strokePath(waveform, juce::PathStrokeType(2.0f));
}

void VanGUI::resized()
{
    // Position your child components here, e.g.:
    // exampleSlider.setBounds (10, 10, getWidth() - 20, 40);
}


void VanGUI::timerCallback()
{
    repaint(); // paints 60 fps
}


