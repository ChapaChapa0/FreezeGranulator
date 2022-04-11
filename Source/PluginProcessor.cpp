/*
  ==============================================================================

    Author:  Chapa

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>

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
        std::make_unique<juce::AudioParameterFloat>("tune", "Coarse Pitch Tunning", juce::NormalisableRange<float>(0.0f, 48.0f, 1.0f), 24.0f),
        std::make_unique<juce::AudioParameterFloat>("fine", "Fine Pitch Tunning", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randTune", "Amount Random Tunning", juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("density", "Density of Grains", juce::NormalisableRange<float>(1.0f, 1000.0f, 0.1f, 1.0f), 10.0f),
        std::make_unique<juce::AudioParameterFloat>("randDensity", "Amount Random Density", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("position", "Position in Sample", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randPosition", "Amount Random Position", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("length", "Length of Grains", juce::NormalisableRange<float>(1.0f, 10000.0f, 0.1f, 0.4f), 50.0f),
        std::make_unique<juce::AudioParameterFloat>("randLength", "Amount Random Length", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterFloat>("level", "Level of Grains", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("randLevel", "Amount Random Level", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),

        std::make_unique<juce::AudioParameterBool>("envelope1", "Envelope 1 Grains", true),
        std::make_unique<juce::AudioParameterBool>("envelope2", "Envelope 2 Grains", false),
        std::make_unique<juce::AudioParameterBool>("envelope3", "Envelope 3 Grains", false),
        std::make_unique<juce::AudioParameterBool>("envelope4", "Envelope 4 Grains", false),

        std::make_unique <juce::AudioParameterChoice>("direction", "Direction Grains", juce::StringArray("Forward", "Backward", "Random"), 0),

        }), juce::Thread("Background Thread")
#endif
{
    time = 0;
    nextGrainOnset = 0;
    sampleRate = 44100;

    formatManager.registerBasicFormats();
    startThread();
}

ChapaGranulatorAudioProcessor::~ChapaGranulatorAudioProcessor()
{
    stopThread(4000);
    clearBuffer();
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
void ChapaGranulatorAudioProcessor::prepareToPlay (double initSampleRate, int samplesPerBlock)
{
    sampleRate = initSampleRate;
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto retainedCurrentBuffer = [&]() -> ReferenceCountedBuffer::Ptr
    {
        const juce::SpinLock::ScopedTryLockType lock(mutex);

        if (lock.isLocked())
            return currentBuffer;

        return nullptr;
    }();

    if (retainedCurrentBuffer == nullptr)
    {
        buffer.clear();
        return;
    }

    auto* currentAudioSampleBuffer = retainedCurrentBuffer->getAudioSampleBuffer();
    auto position = retainedCurrentBuffer->position;

    const int numSamplesInBlock = buffer.getNumSamples();
    const int numChannelsInBlock = buffer.getNumChannels();
    const int numSamplesInFile = currentAudioSampleBuffer->getNumSamples();

    if (numSamplesInBlock == 0) return;

    for (int i = 0; i < numSamplesInBlock; ++i)
    {
        for (int j = 0; j < grainArray.size(); ++j)
        {
            grainArray[j].process(buffer, *currentAudioSampleBuffer, numChannelsInBlock, numSamplesInBlock, numSamplesInFile, time);
        }
        ++time;
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

void ChapaGranulatorAudioProcessor::updateFile()
{
    auto file = juce::File(filePath);
    jassert(file != juce::File{});

    std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor(file));

    if (reader != nullptr)
    {
        ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(), (int)reader->numChannels, (int)reader->lengthInSamples);

        reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);

        {
            const juce::SpinLock::ScopedLockType lock(mutex);
            currentBuffer = newBuffer;
        }

        buffers.add(newBuffer);
    }
}

void ChapaGranulatorAudioProcessor::addGrain()
{

}

void ChapaGranulatorAudioProcessor::run()
{
    while (!threadShouldExit())
    {
        checkForBuffersToFree();

        // Delete grains
        if (grainArray.size() > 0) {
            for (int i = grainArray.size() - 1; i >= 0; --i) {
                // Check if the grain has ended
                long long int grainEnd = grainArray[i].onset + grainArray[i].length;
                bool hasEnded = grainEnd < time;

                if (hasEnded)
                {
                    grainArray.remove(i);
                    numGrains++;
                }
            }
        }

        // Add grains
        int numGrainsToAdd = numGrains - grainArray.size();
        for (int i = 0; i < numGrainsToAdd; ++i)
        {
            if (grainArray.size() < 10)
            {
                //float level = *(parameters.getRawParameterValue("level"));
                //float position = *(parameters.getRawParameterValue("position"));
                //float length = *(parameters.getRawParameterValue("length"));
                //float randLevel = *(parameters.getRawParameterValue("randLevel"));
                //float randPosition = *(parameters.getRawParameterValue("randPosition"));
                //float randLength = *(parameters.getRawParameterValue("randLength"));
                //int envelopeId = (int)*(parameters.getRawParameterValue("envelopeId"));

                //int lengthInSamples = int(length / 1000.0 * sampleRate);
                //long long int onset = time;

                //Grain grain = *new Grain(onset, lengthInSamples, level, position, envelopeId);
                //grainArray.add(grain);
                std::cout << "Ok Ok" << std::endl;
            }
        }

        wait(100);
    }
}

void ChapaGranulatorAudioProcessor::checkForBuffersToFree()
{
    for (auto i = buffers.size(); --i >= 0;)                           // [1]
    {
        ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i)); // [2]

        if (buffer->getReferenceCount() == 2)                          // [3]
            buffers.remove(i);
    }
}

void ChapaGranulatorAudioProcessor::clearBuffer()
{
    const juce::SpinLock::ScopedLockType lock(mutex);
    currentBuffer = nullptr;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChapaGranulatorAudioProcessor();
}
