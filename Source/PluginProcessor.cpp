/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LPFAudioProcessor::LPFAudioProcessor()
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
    , params(apvts)
{
}

LPFAudioProcessor::~LPFAudioProcessor()
{
}

//==============================================================================
const juce::String LPFAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LPFAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LPFAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LPFAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LPFAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LPFAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LPFAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LPFAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LPFAudioProcessor::getProgramName (int index)
{
    return {};
}

void LPFAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LPFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.reset();
    params.update();

    lowPass[0].init((double)sampleRate);
    lowPass[1].init((double)sampleRate);

    lowPass[0].setFreq(300.0f);
    lowPass[0].setQ(10);

    lowPass[1].setFreq(300.0f);
    lowPass[1].setQ(10);
}

void LPFAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LPFAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LPFAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.update();

    if (params.freq != lastFreq)
    {
        lastFreq = params.freq;
        lowPass[0].setFreq(params.freq);
        lowPass[1].setFreq(params.freq);
    }

    if (params.qf != lastQf)
    {
        lastQf = params.qf;
        lowPass[0].setQ(params.qf);
        lowPass[1].setQ(params.qf);
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* inputData = buffer.getReadPointer(channel);
        float* outputData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            outputData[sample] = lowPass[channel].process(inputData[sample]);
        }
    }
}

//==============================================================================
bool LPFAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LPFAudioProcessor::createEditor()
{
    return new LPFAudioProcessorEditor (*this);
}

//==============================================================================
void LPFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void LPFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LPFAudioProcessor();
}
