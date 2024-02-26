/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SostenutoPedalAudioProcessorEditor::SostenutoPedalAudioProcessorEditor (SostenutoPedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    startTimerHz(60);

    resetButton.setBounds(45, 20, 60, 20);
    addAndMakeVisible(resetButton);

    sostenutoButton.setEnabled(false);
    sostenutoButton.setBounds(45, 60, 250, 20);
    addAndMakeVisible(sostenutoButton);

}

SostenutoPedalAudioProcessorEditor::~SostenutoPedalAudioProcessorEditor()
{
}

//==============================================================================
void SostenutoPedalAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
/*
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
*/
}

void SostenutoPedalAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SostenutoPedalAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    audioProcessor.initialize();
}

void SostenutoPedalAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.isSostenutoPedalOn)
        sostenutoButton.setToggleState(true, false);
    else
        sostenutoButton.setToggleState(false, false);
}