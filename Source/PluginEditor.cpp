/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SmartPedalAudioProcessorEditor::SmartPedalAudioProcessorEditor (SmartPedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), vanGUI(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(vanGUI);
    vanGUI.initialize();

    setSize (400, 300);
}

SmartPedalAudioProcessorEditor::~SmartPedalAudioProcessorEditor()
{
}

//==============================================================================
void SmartPedalAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // g.setColour (juce::Colours::white);
    // g.setFont (juce::FontOptions (15.0f));
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SmartPedalAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    vanGUI.setBounds(getLocalBounds());

      // vanGUI.setBounds(getLocalBounds().reduced(10));


}
