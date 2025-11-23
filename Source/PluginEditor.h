/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class LPFAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LPFAudioProcessorEditor (LPFAudioProcessor&);
    ~LPFAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LPFAudioProcessor& audioProcessor;

    RotaryKnob freqKnob{ "Freq", audioProcessor.apvts, freqParamID, 70 };
    RotaryKnob qfKnob{ "Qf", audioProcessor.apvts, qfParamID, 70 };

    MainLookAndFeel mainLF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LPFAudioProcessorEditor)
};
