/*
  ==============================================================================

    Parameters.cpp
    Created: 23 Nov 2025 12:47:58am
    Author:  dkuzn

  ==============================================================================
*/

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, freqParamID, freqParam);
    castParameter(apvts, qfParamID, qfParam);

    update();
}

void Parameters::reset()
{
    freq = 0.0f;
    qf = 0.0f;
}

static juce::String stringGain2f(float value, int)
{
    return juce::String(value, 2);
}

static juce::String stringFromHz(float value, int)
{
    if (value < 1000.0f) {
        return juce::String(int(value)) + " Hz";
    }
    else if (value < 10000.0f) {
        return juce::String(value / 1000.0f, 2) + " k";
    }
    else {
        return juce::String(value / 1000.0f, 1) + " k";
    }
}

static float hzFromString(const juce::String& str)
{
    float value = str.getFloatValue();
    if (value < 20.0f) {
        return value * 1000.0f;
    }
    return value;
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        freqParamID,
        "Freq cut",
        juce::NormalisableRange<float> { 0.0f, 20000.0f },
        100.0f,
        juce::AudioParameterFloatAttributes()
        .withStringFromValueFunction(stringFromHz)
        .withValueFromStringFunction(hzFromString)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        qfParamID,
        "Q factor",
        juce::NormalisableRange<float> { 0.1f, 20.0f },
        10.0f,
        juce::AudioParameterFloatAttributes()
        .withStringFromValueFunction(stringGain2f)
    ));

    return layout;
}

void Parameters::update() noexcept
{
    freq = freqParam->get();
    qf = qfParam->get();
}
