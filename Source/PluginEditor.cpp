/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginEditor.h"

//==============================================================================
SmartPedalAudioProcessorEditor::SmartPedalAudioProcessorEditor(SmartPedalAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), vanGUI(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(vanGUI);
    
    
    addAndMakeVisible(Distortion);
    Distortion.setButtonText("Distortion");
    Distortion.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    Distortion.setClickingTogglesState(true);
    Distortion.addListener(this);
    
    vanGUI.initialize();
    

    setSize(400, 300);
}

SmartPedalAudioProcessorEditor::~SmartPedalAudioProcessorEditor()
{
}

//==============================================================================
void SmartPedalAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // g.setColour (juce::Colours::white);
    // g.setFont (juce::FontOptions (15.0f));
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

// add member method for getting toggle state
void SmartPedalAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &Distortion)
    {
        
        bool isOn = button->getToggleState();
        if(isOn){
            
            button->setColour(juce::TextButton::buttonOnColourId,juce::Colours::green);
            
            audioProcessor.setDistortionEnabled(isOn);
        }
        else{
            button->setColour(juce::TextButton::buttonColourId,juce::Colours::red);
            
            audioProcessor.setDistortionEnabled(false);
            
           
        }
    }
}

void SmartPedalAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    auto guiArea = area.removeFromLeft(area.getWidth() * 0.75f);
        
        vanGUI.setBounds(guiArea);
        Distortion.setBounds(area);

    // vanGUI.setBounds(getLocalBounds().reduced(10));


}
