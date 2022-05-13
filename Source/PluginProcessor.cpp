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
        std::make_unique<juce::AudioParameterInt>("maxGrains", "Number Max of Grains", 10, 1000, 100),

        std::make_unique<juce::AudioParameterFloat>("transpose", "Transposition of Sample", juce::NormalisableRange<float>(-2400.0f, 2400.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randTranspose", "Amount Random Transposition", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("inertiaTranspose", "Amount Transposition Inertia", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterBool>("inertiaTransposeOff", "Inertia Transpose Off", true),
        std::make_unique<juce::AudioParameterBool>("inertiaTransposeNote", "Inertia Transpose Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("inertiaTransposeHz", "Inertia Transpose Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("density", "Density of Grains", juce::NormalisableRange<float>(1.0f, 100.0f, 0.01f, 0.4f), 10.0f),
        std::make_unique<juce::AudioParameterFloat>("randDensity", "Amount Random Density", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("inertiaDensity", "Amount Density Inertia", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterBool>("inertiaDensityOff", "Inertia Density Off", true),
        std::make_unique<juce::AudioParameterBool>("inertiaDensityNote", "Inertia Density Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("inertiaDensityHz", "Inertia Density Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("position", "Position in Sample", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randPosition", "Amount Random Position", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("inertiaPosition", "Amount Position Inertia", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterBool>("inertiaPositionOff", "Inertia Position Off", true),
        std::make_unique<juce::AudioParameterBool>("inertiaPositionNote", "Inertia Position Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("inertiaPositionHz", "Inertia Position Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("length", "Length of Grains", juce::NormalisableRange<float>(1.0f, 10000.0f, 0.1f, 0.3f), 50.0f),
        std::make_unique<juce::AudioParameterFloat>("randLength", "Amount Random Length", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("inertiaLength", "Amount Length Inertia", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterBool>("inertiaLengthOff", "Inertia Length Off", true),
        std::make_unique<juce::AudioParameterBool>("inertiaLengthNote", "Inertia Length Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("inertiaLengthHz", "Inertia Length Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("level", "Level of Grains", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("randLevel", "Amount Random Level", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("inertiaLevel", "Amount Level Inertia", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterBool>("inertiaLevelOff", "Inertia Level Off", true),
        std::make_unique<juce::AudioParameterBool>("inertiaLevelNote", "Inertia Level Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("inertiaLevelHz", "Inertia Level Frequency Mode", false),

        std::make_unique <juce::AudioParameterFloat>("panning", "Panning of Grains", juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique <juce::AudioParameterFloat>("randPanning", "Amount Random Panning", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique <juce::AudioParameterFloat>("inertiaPanning", "Amount Panning Inertia", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterBool>("inertiaPanningOff", "Inertia Panning Off", true),
        std::make_unique<juce::AudioParameterBool>("inertiaPanningNote", "Inertia Panning Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("inertiaPanningHz", "Inertia Panning Frequency Mode", false),

        std::make_unique<juce::AudioParameterBool>("envelopeSine", "Grain Envelope Sine", true),
        std::make_unique<juce::AudioParameterBool>("envelopeTriangle", "Grain Envelope Triangle", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRectangle", "Grain Envelope Rectangle", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRampUp", "Grain Envelope Ramp Up", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRampDown", "Grain Envelope Ramp Down", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRandom", "Grain Envelope Random", false),

        std::make_unique<juce::AudioParameterBool>("directionForward", "Direction Grains Forward", true),
        std::make_unique<juce::AudioParameterBool>("directionBackward", "Direction Grains Backward", false),
        std::make_unique<juce::AudioParameterBool>("directionRandom", "Direction Grains Random", false),
        
        //std::make_unique <juce::AudioParameterChoice>("direction", "Direction Grains", juce::StringArray("Forward", "Backward", "Random"), 0),

        }), juce::Thread("Background Thread")
#endif
{
    time = 0;
    sampleRate = 44100;

    juce::Logger::setCurrentLogger(fileLog);

    // Init all grains and inertia parameters
    grainLength = 50.0f, grainPosition = 0.0f, grainTranspose = 0.0f, grainDensity = 10.0f, grainLevel = 0.0f, grainPanning = 0.0f;
    timeLength = 0, timePosition = 0, timeTranspose = 0, timeDensity = 0, timeLevel = 0, timePanning = 0;

    formatManager.registerBasicFormats();
    startThread();
}

ChapaGranulatorAudioProcessor::~ChapaGranulatorAudioProcessor()
{
    stopThread(4000);
    clearBuffer();

    juce::Logger::setCurrentLogger(nullptr);
    delete fileLog;
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

    // Update BPM value
    auto playHead = getPlayHead();
    if (playHead != nullptr)
    {
        playHead->getCurrentPosition(currentPositionInfo);
        if (currentPositionInfo.bpm != myBPM)
            myBPM = currentPositionInfo.bpm;
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    processMidi(midiMessages);

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

//==============================================================================

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

                if (hasEnded) grainArray.remove(i);
            }
        }

        // Handle MIDI
        juce::Array<juce::Array<int>> activeNotes;

        for (int i = 0; i < 128; i++) {
            if (midiNotes[i] > 0) {
                activeNotes.add(juce::Array<int> {i, midiNotes[i] });
            }
        }

        // Add grains
        int maxGrains = (int) *(parameters.getRawParameterValue("maxGrains"));
        if (currentBuffer != nullptr && activeNotes.size() > 0 && grainArray.size() < maxGrains)
        {
            // Get time value before adding grain
            long long int debutTime = time;

            // Get grain envelope
            int grainEnvelopeId = -1;
            int i = 0;
            do
            {
                float envelope_i = *(parameters.getRawParameterValue(envelopesId[i]));
                if (envelope_i > 0.5f) grainEnvelopeId = i;
                ++i;
            } while (i < envelopesId.size() && grainEnvelopeId < 0);
            if (grainEnvelopeId == 5) grainEnvelopeId = random.nextInt(5);

            // Compute transpose grain parameter according to random and inertia values
            if (*(parameters.getRawParameterValue("inertiaTransposeOff")) > 0.5f || time > timeTranspose)
            {
                float transpose = *(parameters.getRawParameterValue("transpose"));
                float randTranspose = *(parameters.getRawParameterValue("randTranspose"));
                grainTranspose = transpose + (2.0f * (0.5f - random.nextFloat()) * randTranspose / 100.0f) * 2400.0f;
                grainTranspose = juce::jmin(2400.0f, juce::jmax(-2400.0f, grainTranspose));
            }
            if (time > timeTranspose)
            {
                float inertiaTranspose = *(parameters.getRawParameterValue("inertiaTranspose"));
                if (*(parameters.getRawParameterValue("inertiaTransposeNote")) > 0.5f && myBPM > 0)
                {
                    timeTranspose = time + int(inertiaTranspose * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("inertiaTransposeHz")) > 0.5f)
                {
                    timeTranspose = time + int(1 / inertiaTranspose * sampleRate);
                }
            }

            // Compute level grain parameter according to random and inertia values
            if (*(parameters.getRawParameterValue("inertiaLevelOff")) > 0.5f || time > timeLevel)
            {
                float level = *(parameters.getRawParameterValue("level"));
                float randLevel = *(parameters.getRawParameterValue("randLevel"));
                grainLevel = level / 100.0f + (2.0f * (0.5f - random.nextFloat()) * randLevel / 100.0f);
                grainLevel = juce::jmin(1.0f, juce::jmax(0.0f, grainLevel));
            }
            if (time > timeLevel)
            {
                float inertiaLevel = *(parameters.getRawParameterValue("inertiaLevel"));
                if (*(parameters.getRawParameterValue("inertiaLevelNote")) > 0.5f && myBPM > 0)
                {
                    timeLevel = time + int(inertiaLevel * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("inertiaLevelHz")) > 0.5f)
                {
                    timeLevel = time + int(1 / inertiaLevel * sampleRate);
                }
            }

            // Compute position grain parameter according to random and inertia values
            if (*(parameters.getRawParameterValue("inertiaPositionOff")) > 0.5f || time > timePosition)
            {
                float position = *(parameters.getRawParameterValue("position"));
                float randPosition = *(parameters.getRawParameterValue("randPosition"));
                grainPosition = position + (2.0f * (0.5f - random.nextFloat()) * randPosition / 100.0f);
                grainPosition = juce::jmin(1.0f, juce::jmax(0.0f, grainPosition));
            }
            if (time > timePosition)
            {
                float inertiaPosition = *(parameters.getRawParameterValue("inertiaPosition"));
                if (*(parameters.getRawParameterValue("inertiaPositionNote")) > 0.5f && myBPM > 0)
                {
                    timePosition = time + int(inertiaPosition * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("inertiaPositionHz")) > 0.5f)
                {
                    timePosition = time + int(1 / inertiaPosition * sampleRate);
                }
            }

            // Compute length grain parameter according to random and inertia values
            if (*(parameters.getRawParameterValue("inertiaLengthOff")) > 0.5f || time > timeLength)
            {
                float length = *(parameters.getRawParameterValue("length"));
                float randLength = *(parameters.getRawParameterValue("randLength"));
                grainLength = length + (2.0f * (0.5f - random.nextFloat()) * (float)pow(randLength / 100.0, 1.0 / 0.3)) * 10000.0f;
                grainLength = juce::jmin(10000.0f, juce::jmax(1.0f, grainLength));
            }
            if (time > timeLength)
            {
                float inertiaLength = *(parameters.getRawParameterValue("inertiaLength"));
                if (*(parameters.getRawParameterValue("inertiaLengthNote")) > 0.5f && myBPM > 0)
                {
                    timeLength = time + int(inertiaLength * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("inertiaLengthHz")) > 0.5f)
                {
                    timeLength = time + int(1 / inertiaLength * sampleRate);
                }
            }

            // Compute density grain parameter according to random and inertia values
            if (*(parameters.getRawParameterValue("inertiaDensityOff")) > 0.5f || time > timeDensity)
            {
                float density = *(parameters.getRawParameterValue("density"));
                float randDensity = *(parameters.getRawParameterValue("randDensity"));
                grainDensity = density + (2.0f * (0.5f - random.nextFloat()) * (float)pow(randDensity / 100.0, 1.0 / 0.4)) * 100.0f;
                grainDensity = juce::jmin(100.0f, juce::jmax(1.0f, grainDensity));
            }
            if (time > timeDensity)
            {
                float inertiaDensity = *(parameters.getRawParameterValue("inertiaDensity"));
                if (*(parameters.getRawParameterValue("inertiaDensityNote")) > 0.5f && myBPM > 0)
                {
                    timeDensity = time + int(inertiaDensity * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("inertiaDensityHz")) > 0.5f)
                {
                    timeDensity = time + int(1 / inertiaDensity * sampleRate);
                }
            }

            // Compute panning grain parameter according to random and inertia values
            if (*(parameters.getRawParameterValue("inertiaPanningOff")) > 0.5f || time > timePanning)
            {
                float panning = *(parameters.getRawParameterValue("panning"));
                float randPanning = *(parameters.getRawParameterValue("randPanning"));
                grainPanning = panning + (2.0f * (0.5f - random.nextFloat()) * randPanning);
                grainPanning = juce::jmin(100.0f, juce::jmax(-100.0f, grainPanning));
            }
            if (time > timePanning)
            {
                float inertiaPanning = *(parameters.getRawParameterValue("inertiaPanning"));
                if (*(parameters.getRawParameterValue("inertiaPanningNote")) > 0.5f && myBPM > 0)
                {
                    timePanning = time + int(inertiaPanning * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("inertiaPanningHz")) > 0.5f)
                {
                    timePanning = time + int(1 / inertiaPanning * sampleRate);
                }
            }

            // Convert some parameters from time to number of samples
            float grainRate = pow(10, grainTranspose / (1200.0 * 3.322038403));
            int grainLengthInSamples = int(grainLength / 1000.0 * sampleRate);

            // Compute onset time for the grain
            long long int onset = time + delayOnset;

            // Generate grain and add it to the array of grains
            Grain grain = *new Grain(onset, grainLengthInSamples, grainLevel, grainPosition, grainPanning, grainRate, grainEnvelopeId);
            grainArray.add(grain);

            // Wait according to the ponctual density computed (density = number of grains generated per second)
            int timeToWait = juce::jmax(0, int(1000 / grainDensity - (time - debutTime) / sampleRate * 1000));
            wait(timeToWait);
        }
        else
        {
            // Wait for a sample to be loaded or for a note to be played or for grains to be deleted
            wait(100);
        }
    }
}

//==============================================================================

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

void ChapaGranulatorAudioProcessor::processMidi(juce::MidiBuffer& midiMessages)
{
    juce::MidiBuffer::Iterator i(midiMessages);
    juce::MidiMessage message;
    int time;

    while (i.getNextEvent(message, time))
    {
        if (message.isNoteOn()) {
            midiNotes[message.getNoteNumber()] = message.getVelocity();
            notify();
        }
        if (message.isNoteOff()) {
            midiNotes[message.getNoteNumber()] = 0;
        }
        if (message.isAllNotesOff()) {
            for (int i = 0; i < 128; ++i)
                midiNotes[i] = 0;
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChapaGranulatorAudioProcessor();
}
