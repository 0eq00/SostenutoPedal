/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
/*
class SostenutoPedalAudioProcessorEditor  : public juce::AudioProcessorEditor
*/
class SostenutoPedalAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Button::Listener, private juce::Timer
{
public:
    SostenutoPedalAudioProcessorEditor (SostenutoPedalAudioProcessor&);
    ~SostenutoPedalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SostenutoPedalAudioProcessor& audioProcessor;

    juce::TextButton resetButton{ "Rset" };
    juce::ToggleButton sostenutoButton{ "Sostenuto Pedal" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SostenutoPedalAudioProcessorEditor)
};
