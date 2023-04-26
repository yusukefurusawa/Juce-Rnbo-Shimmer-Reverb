/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamIDs.h"
#include "RNBO_TimeConverter.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto msFormat = [](float value, int)
    {
        if (value < 100.0f)
            return juce::String (value, 1) + " ms";
        else
            return juce::String (std::roundf (value)) + " ms";
    };
    
    auto percentFormat = [](float value, int)
    {
         if (value < 10.0f)
             return juce::String (value, 2) + " %";
         else if (value < 100.0f)
             return juce::String (value, 1) + " %";
         else
             return juce::String (value, 0) + " %";
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::size, 1 },
                                                             ParamIDs::size,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             75.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
  layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::decay, 1 },
                                                           ParamIDs::decay,
                                                           juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                           70.0f,
                                                           juce::String(),
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           percentFormat,
                                                           nullptr));
  layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::damp, 1 },
                                                           ParamIDs::damp,
                                                           juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                           50.0f,
                                                           juce::String(),
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           percentFormat,
                                                           nullptr));
  layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::jitter, 1 },
                                                             ParamIDs::jitter,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
  layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::diff, 1 },
                                                           ParamIDs::diff,
                                                           juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                           20.0f,
                                                           juce::String(),
                                                           juce::AudioProcessorParameter::genericParameter,
                                                           percentFormat,
                                                           nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::pitch, 1 },
                                                              ParamIDs::pitch,
                                                              juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f, 1.0f),
                                                              0.0f,
                                                              juce::String(),
                                                              juce::AudioProcessorParameter::genericParameter,
                                                              [](float value, int) {
                                                                 return juce::String (value, 1) + " st"; },
                                                              nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::window, 1 },
                                                             ParamIDs::window,
                                                             juce::NormalisableRange<float> (25.0f, 250.0f, 0.01f, 0.405f),
                                                             125.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::delay, 1 },
                                                             ParamIDs::delay,
                                                             juce::NormalisableRange<float> (0.0f, 1000.0f, 0.01f, 0.405f),
                                                             0.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::feedback, 1 },
                                                             ParamIDs::feedback,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::chorus, 1 },
                                                             ParamIDs::chorus,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::revvol, 1 },
                                                             ParamIDs::revvol,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::pitchvol, 1 },
                                                             ParamIDs::pitchvol,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             0.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::octvol, 1 },
                                                             ParamIDs::octvol,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::octdamp, 1 },
                                                             ParamIDs::octdamp,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::mix, 1 },
                                                             ParamIDs::mix,
                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::interval, 1 },
//                                                             ParamIDs::interval,
//                                                             juce::NormalisableRange<float> (10.0f, 500.0f, 0.01f, 0.405f),
//                                                             100.0f,
//                                                             juce::String(),
//                                                             juce::AudioProcessorParameter::genericParameter,
//                                                             msFormat,
//                                                             nullptr));
//
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::pitch, 1 },
//                                                              ParamIDs::pitch,
//                                                              juce::NormalisableRange<float> (-12.0f, 12.0f, 0.1f, 1.0f),
//                                                              0.0f,
//                                                              juce::String(),
//                                                              juce::AudioProcessorParameter::genericParameter,
//                                                              [](float value, int) {
//                                                                 return juce::String (value, 1) + " st"; },
//                                                              nullptr));
//
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::grainPos, 1 },
//                                                             ParamIDs::grainPos,
//                                                             juce::NormalisableRange<float> (10.0f, 500.0f, 1.0f, 0.405f),
//                                                             100.0f,
//                                                             juce::String(),
//                                                             juce::AudioProcessorParameter::genericParameter,
//                                                             msFormat,
//                                                             nullptr));
//
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::grainSize, 1 },
//                                                             ParamIDs::grainSize,
//                                                             juce::NormalisableRange<float> (10.0f, 500.0f, 0.01f, 0.405f),
//                                                             100.0f,
//                                                             juce::String(),
//                                                             juce::AudioProcessorParameter::genericParameter,
//                                                             msFormat,
//                                                             nullptr));
//
//    auto percentFormat = [](float value, int)
//    {
//         if (value < 10.0f)
//             return juce::String (value, 2) + " %";
//         else if (value < 100.0f)
//             return juce::String (value, 1) + " %";
//         else
//             return juce::String (value, 0) + " %";
//    };
//
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::mix, 1 },
//                                                             ParamIDs::mix,
//                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
//                                                             50.0f,
//                                                             juce::String(),
//                                                             juce::AudioProcessorParameter::genericParameter,
//                                                             percentFormat,
//                                                             nullptr));
//
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::width, 1 },
//                                                             ParamIDs::width,
//                                                             juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f, 1.0f),
//                                                             50.0,
//                                                             juce::String(),
//                                                             juce::AudioProcessorParameter::genericParameter,
//                                                             percentFormat,
//                                                             nullptr));
//
//    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::gain, 1 },
//                                                             ParamIDs::gain,
//                                                             juce::NormalisableRange<float> (-36.0f, 12.0f, 0.1f, 2.4f),
//                                                             0.0f,
//                                                             juce::String(),
//                                                             juce::AudioProcessorParameter::genericParameter,
//                                                             [](float value, int) {
//                                                             if (-10.0f < value && value < 10.0f)
//                                                                 return juce::String (value, 1) + " dB";
//                                                             else
//                                                                 return juce::String (std::roundf (value), 0) + " dB"; },
//                                                             nullptr));

    return layout;
}

//==============================================================================
JuceRnboTemplateAudioProcessor::JuceRnboTemplateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    for (RNBO::ParameterIndex i = 0; i < rnboObject.getNumParameters(); ++i)
    {
        RNBO::ParameterInfo info;
        rnboObject.getParameterInfo (i, &info);

        if (info.visible)
        {
            auto paramID = juce::String (rnboObject.getParameterId (i));

            // Each apvts parameter id and range must be the same as the rnbo param object's.
            // If you hit this assertion then you need to fix the incorrect id in ParamIDs.h.
            jassert (apvts.getParameter (paramID) != nullptr);

            // If you hit these assertions then you need to fix the incorrect apvts
            // parameter range in createParameterLayout().
            jassert (static_cast<float> (info.min) == apvts.getParameterRange (paramID).start);
            jassert (static_cast<float> (info.max) == apvts.getParameterRange (paramID).end);

            apvtsParamIdToRnboParamIndex[paramID] = i;

            apvts.addParameterListener (paramID, this);
            rnboObject.setParameterValue (i, apvts.getRawParameterValue (paramID)->load());
        }
    }
}

JuceRnboTemplateAudioProcessor::~JuceRnboTemplateAudioProcessor()
{
}

//==============================================================================
const juce::String JuceRnboTemplateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceRnboTemplateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceRnboTemplateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuceRnboTemplateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuceRnboTemplateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceRnboTemplateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceRnboTemplateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceRnboTemplateAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String JuceRnboTemplateAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void JuceRnboTemplateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void JuceRnboTemplateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    rnboObject.prepareToProcess (sampleRate, static_cast<size_t> (samplesPerBlock));
}

void JuceRnboTemplateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceRnboTemplateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JuceRnboTemplateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto bufferSize = buffer.getNumSamples();
    
    // fill midi input
    RNBO::TimeConverter timeConverter (rnboObject.getSampleRate(), rnboObject.getCurrentTime());

    midiInput.clear();  // make sure midi input starts clear
    for (auto meta: midiMessages)
    {
        RNBO::MillisecondTime time = timeConverter.convertSampleOffsetToMilliseconds (meta.samplePosition);
        midiInput.addEvent (RNBO::MidiEvent (time, 0, meta.data, static_cast<RNBO::Index> (meta.numBytes)));
    }

    rnboObject.prepareToProcess (getSampleRate(),
                                 static_cast<size_t> (bufferSize));

    rnboObject.process (buffer.getArrayOfWritePointers(),
                        static_cast<RNBO::Index> (buffer.getNumChannels()),
                        buffer.getArrayOfWritePointers(),
                        static_cast<RNBO::Index> (buffer.getNumChannels()),
                        static_cast<RNBO::Index> (bufferSize),
                        &midiInput, &midiOutput);
                        
    // consume midi output
    midiMessages.clear(); // clear the input that we consumed above so juce doesn't get confused
    if (!midiOutput.empty())
    {
        std::for_each (midiOutput.begin(),
                       midiOutput.end(),
                       [&timeConverter, &midiMessages](const RNBO::MidiEvent& ev) {
                           auto sampleNumber = timeConverter.convertMillisecondsToSampleOffset (ev.getTime());
                           auto midiMessage = juce::MidiMessage (ev.getData(),
                                                                  static_cast<int> (ev.getLength()));
                           midiMessages.addEvent (midiMessage, static_cast<int> (sampleNumber));
                       });
        midiOutput.clear();
    }
}

//==============================================================================
bool JuceRnboTemplateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JuceRnboTemplateAudioProcessor::createEditor()
{
//    return new JuceRnboTemplateAudioProcessorEditor (*this, apvts);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void JuceRnboTemplateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void JuceRnboTemplateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData (data, static_cast<size_t> (sizeInBytes));

    if (tree.isValid())
        apvts.replaceState (tree);
}

void JuceRnboTemplateAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    rnboObject.setParameterValue (apvtsParamIdToRnboParamIndex[parameterID], newValue);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceRnboTemplateAudioProcessor();
}
