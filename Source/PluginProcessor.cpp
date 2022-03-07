/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChapaGranulatorAudioProcessor::ChapaGranulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, juce::Identifier("ChapaGranulator"),
        {
        std::make_unique<juce::AudioParameterFloat>("attack", "Attack", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("release", "Release", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.0f),

        std::make_unique<juce::AudioParameterFloat>("tune", "Coarse Pitch Tunning", juce::NormalisableRange<float>(0.0f, 48.0f, 1.0f), 24.0f),
        std::make_unique<juce::AudioParameterFloat>("fine", "Fine Pitch Tunning", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randTune", "Amount Random Tunning", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("grains", "Number of Grains", juce::NormalisableRange<float>(0.1f, 100.0f, 0.1f), 5.0f),
        std::make_unique<juce::AudioParameterFloat>("randGrains", "Amount Random Grains", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("position", "Position in Sample", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randPosition", "Amount Random Position", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("length", "Length of Grains", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 10.0f),
        std::make_unique<juce::AudioParameterFloat>("randLength", "Amount Random Length", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("level", "Level of Grains", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("randLevel", "Amount Random Level", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterBool>("envelope1", "Envelope 1 Grains", true),
        std::make_unique<juce::AudioParameterBool>("envelope2", "Envelope 2 Grains", false),
        std::make_unique<juce::AudioParameterBool>("envelope3", "Envelope 3 Grains", false),
        std::make_unique<juce::AudioParameterBool>("envelope4", "Envelope 4 Grains", false),

        std::make_unique<juce::AudioParameterBool>("forward", "Direction Grains Forward", true),
        std::make_unique<juce::AudioParameterBool>("backward", "Direction Grains Backward", false),
        std::make_unique<juce::AudioParameterBool>("random", "Direction Grains Random", false),
        //std::make_unique <juce::AudioParameterChoice>("direction", "Direction Grains", juce::StringArray("Forward", "Backward", "Random"), 0),

        })
#endif
{

    /////////////////
    
}

ChapaGranulatorAudioProcessor::~ChapaGranulatorAudioProcessor()
{
}

//==============================================================================
const juce::String ChapaGranulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChapaGranulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChapaGranulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChapaGranulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChapaGranulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChapaGranulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChapaGranulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChapaGranulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ChapaGranulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChapaGranulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ChapaGranulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    keyboardState.reset();
}

void ChapaGranulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChapaGranulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ChapaGranulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
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
}

//==============================================================================
bool ChapaGranulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ChapaGranulatorAudioProcessor::createEditor()
{
    return new ChapaGranulatorAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void ChapaGranulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ChapaGranulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
void ChapaGranulatorAudioProcessor::updateValue()
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChapaGranulatorAudioProcessor();
}
