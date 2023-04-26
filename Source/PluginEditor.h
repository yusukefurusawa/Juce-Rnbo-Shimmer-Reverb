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
class JuceRnboTemplateAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JuceRnboTemplateAudioProcessorEditor (JuceRnboTemplateAudioProcessor& p,
                                           juce::AudioProcessorValueTreeState& apvts);
    ~JuceRnboTemplateAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JuceRnboTemplateAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JuceRnboTemplateAudioProcessorEditor)
};
