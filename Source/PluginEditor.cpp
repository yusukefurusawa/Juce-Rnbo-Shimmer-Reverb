/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceRnboTemplateAudioProcessorEditor::JuceRnboTemplateAudioProcessorEditor (JuceRnboTemplateAudioProcessor& p,
                                                                              juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
}

JuceRnboTemplateAudioProcessorEditor::~JuceRnboTemplateAudioProcessorEditor()
{
}

//==============================================================================
void JuceRnboTemplateAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (25.0f);
    g.drawFittedText ("AMCJ #32", getLocalBounds(), juce::Justification::centred, 1);
}

void JuceRnboTemplateAudioProcessorEditor::resized()
{
}
