/*
  ==============================================================================

    VanGUI.cpp
    Created: 16 Mar 2025 5:30:07pm
    Author:  wllun

  ==============================================================================
*/

#include "VanGUI.h"
#include "PluginProcessor.h"

VanGUI::VanGUI(SmartPedalAudioProcessor& p) : processor(p)
{

    // Constructor: You can create child components here if you like.
    startTimerHz(120);
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

    // Example placeholder text:
    g.setColour(juce::Colours::white);
    g.drawFittedText("Van's GUI Area", getLocalBounds(), juce::Justification::centred, 1);
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

