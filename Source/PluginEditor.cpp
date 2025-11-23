/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LPFAudioProcessorEditor::LPFAudioProcessorEditor (LPFAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(freqKnob);
    addAndMakeVisible(qfKnob);

    setSize (400, 300);

    setLookAndFeel(&mainLF);
}

LPFAudioProcessorEditor::~LPFAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void LPFAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

}

void LPFAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    auto margin = 0.15f * height;

    freqKnob.setTopLeftPosition(width * 0.33f - 35, margin);
    qfKnob.setTopLeftPosition(width * 0.66f - 35, margin);
}
