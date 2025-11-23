/*
  ==============================================================================

    Parameters.h
    Created: 23 Nov 2025 12:47:58am
    Author:  dkuzn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const juce::ParameterID freqParamID{ "freq", 1 };
const juce::ParameterID qfParamID{ "Q", 1 };

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void reset();
    void update() noexcept;

    float freq = 0.0f;
    float qf = 0.0f;

private:
    juce::AudioParameterFloat* freqParam;
    juce::AudioParameterFloat* qfParam;

    float sampleRate;
    float fN;
};
