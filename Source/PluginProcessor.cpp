/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SostenutoPedalAudioProcessor::SostenutoPedalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    this->initialize();
}

void SostenutoPedalAudioProcessor::initialize()
{
    int i = 0;

    isSostenutoPedalOn = false;
    for (i = 0; i < 127; i++)
    {
        keyOn[i] = false;
        sosOn[i] = false;
    }
}

SostenutoPedalAudioProcessor::~SostenutoPedalAudioProcessor()
{
}

//==============================================================================
const juce::String SostenutoPedalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SostenutoPedalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SostenutoPedalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SostenutoPedalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SostenutoPedalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SostenutoPedalAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SostenutoPedalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SostenutoPedalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SostenutoPedalAudioProcessor::getProgramName (int index)
{
    return {};
}

void SostenutoPedalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SostenutoPedalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SostenutoPedalAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SostenutoPedalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SostenutoPedalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    juce::MidiBuffer processedMidi;
    int i = 0;

    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isSostenutoPedalOn())
        {
            this->isSostenutoPedalOn = true;
            for (i = 0; i < 127; i++)
            {
                sosOn[i] = keyOn[i];
            }
            continue;
        }

        if (message.isSostenutoPedalOff())
        {
            this->isSostenutoPedalOn = false;
            for (i = 0; i < 127; i++)
            {
                if (sosOn[i] && !keyOn[i])
                {
                    processedMidi.addEvent(juce::MidiMessage::noteOff(message.getChannel(), (juce::uint8)i), time);
                }

                sosOn[i] = false;
            }
            continue;
        }

        if (message.isNoteOn())
        {
            keyOn[message.getNoteNumber()] = true;
        }

        if (message.isNoteOff())
        {
            keyOn[message.getNoteNumber()] = false;

            if (sosOn[message.getNoteNumber()])
            {
                continue;
            }
        }

        processedMidi.addEvent(message, time);
    }

    midiMessages.swapWith(processedMidi);

/*
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
*/
}

//==============================================================================
bool SostenutoPedalAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SostenutoPedalAudioProcessor::createEditor()
{
    return new SostenutoPedalAudioProcessorEditor (*this);
}

//==============================================================================
void SostenutoPedalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SostenutoPedalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SostenutoPedalAudioProcessor();
}
