/*
  ==============================================================================

    Author:  Chapa

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>

//==============================================================================
FreezeGranulatorAudioProcessor::FreezeGranulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, juce::Identifier("FreezeGranulator"),
        {
        std::make_unique<juce::AudioParameterInt>("maxGrains", "Number Max of Grains", 10, 1000, 100),

        std::make_unique<juce::AudioParameterFloat>("transpose", "Transposition of Sample", juce::NormalisableRange<float>(-2400.0f, 2400.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randTranspose", "Amount Random Transposition", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("freezeTranspose", "Amount Transposition Freeze", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("freezeTransposeSynched", "Amount Transposition Freeze (Synched)", 0, 9, 4),
        std::make_unique<juce::AudioParameterBool>("freezeTransposeOff", "Freeze Transpose Off", true),
        std::make_unique<juce::AudioParameterBool>("freezeTransposeNote", "Freeze Transpose Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("freezeTransposeHz", "Freeze Transpose Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("density", "Density of Grains", juce::NormalisableRange<float>(1.0f, 100.0f, 0.01f, 0.4f), 10.0f),
        std::make_unique<juce::AudioParameterFloat>("randDensity", "Amount Random Density", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("freezeDensity", "Amount Density Freeze", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("freezeDensitySynched", "Amount Density Freeze (Synched)", 0, 9, 4),
        std::make_unique<juce::AudioParameterBool>("freezeDensityOff", "Freeze Density Off", true),
        std::make_unique<juce::AudioParameterBool>("freezeDensityNote", "Freeze Density Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("freezeDensityHz", "Freeze Density Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("position", "Position in Sample", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("randPosition", "Amount Random Position", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("freezePosition", "Amount Position Freeze", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("freezePositionSynched", "Amount Position Freeze (Synched)", 0, 9, 4),
        std::make_unique<juce::AudioParameterBool>("freezePositionOff", "Freeze Position Off", true),
        std::make_unique<juce::AudioParameterBool>("freezePositionNote", "Freeze Position Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("freezePositionHz", "Freeze Position Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("length", "Length of Grains", juce::NormalisableRange<float>(1.0f, 10000.0f, 0.1f, 0.3f), 50.0f),
        std::make_unique<juce::AudioParameterFloat>("randLength", "Amount Random Length", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("freezeLength", "Amount Length Freeze", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("freezeLengthSynched", "Amount Length Freeze (Synched)", 0, 9, 4),
        std::make_unique<juce::AudioParameterBool>("freezeLengthOff", "Freeze Length Off", true),
        std::make_unique<juce::AudioParameterBool>("freezeLengthNote", "Freeze Length Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("freezeLengthHz", "Freeze Length Frequency Mode", false),

        std::make_unique<juce::AudioParameterFloat>("level", "Level of Grains", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f),
        std::make_unique<juce::AudioParameterFloat>("randLevel", "Amount Random Level", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("freezeLevel", "Amount Level Freeze", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("freezeLevelSynched", "Amount Level Freeze (Synched)", 0, 9, 4),
        std::make_unique<juce::AudioParameterBool>("freezeLevelOff", "Freeze Level Off", true),
        std::make_unique<juce::AudioParameterBool>("freezeLevelNote", "Freeze Level Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("freezeLevelHz", "Freeze Level Frequency Mode", false),

        std::make_unique <juce::AudioParameterFloat>("panning", "Panning of Grains", juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique <juce::AudioParameterFloat>("randPanning", "Amount Random Panning", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f),
        std::make_unique <juce::AudioParameterFloat>("freezePanning", "Amount Panning Freeze", juce::NormalisableRange<float>(0.1f, 100.0f, 0.01f, 0.4f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("freezePanningSynched", "Amount Panning Freeze (Synched)", 0, 9, 4),
        std::make_unique<juce::AudioParameterBool>("freezePanningOff", "Freeze Panning Off", true),
        std::make_unique<juce::AudioParameterBool>("freezePanningNote", "Freeze Panning Note Mode", false),
        std::make_unique<juce::AudioParameterBool>("freezePanningHz", "Freeze Panning Frequency Mode", false),

        std::make_unique<juce::AudioParameterBool>("envelopeSine", "Grain Envelope Sine", true),
        std::make_unique<juce::AudioParameterBool>("envelopeTriangle", "Grain Envelope Triangle", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRectangle", "Grain Envelope Rectangle", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRampUp", "Grain Envelope Ramp Up", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRampDown", "Grain Envelope Ramp Down", false),
        std::make_unique<juce::AudioParameterBool>("envelopeRandom", "Grain Envelope Random", false),

        std::make_unique<juce::AudioParameterBool>("directionForward", "Direction Grains Forward", true),
        std::make_unique<juce::AudioParameterBool>("directionBackward", "Direction Grains Backward", false),
        std::make_unique<juce::AudioParameterBool>("directionRandom", "Direction Grains Random", false),

        }), juce::Thread("Background Thread")
#endif
{
    time = 0;
    sampleRate = 44100;

    juce::Logger::setCurrentLogger(fileLog);

    // Init all grains and freeze parameters
    grainLength = 50.0f, grainPosition = 0.0f, grainTranspose = 0.0f, grainDensity = 10.0f, grainLevel = 0.0f, grainPanning = 0.0f;
    timeLength = -1, timePosition = -1, timeTranspose = -1, timeDensity = -1, timeLevel = -1, timePanning = -1;

    formatManager.registerBasicFormats();
    startThread();
}

FreezeGranulatorAudioProcessor::~FreezeGranulatorAudioProcessor()
{
    stopThread(4000);
    clearBuffer();

    juce::Logger::setCurrentLogger(nullptr);
    delete fileLog;
}

//==============================================================================
const juce::String FreezeGranulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FreezeGranulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FreezeGranulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FreezeGranulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FreezeGranulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FreezeGranulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FreezeGranulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FreezeGranulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FreezeGranulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void FreezeGranulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FreezeGranulatorAudioProcessor::prepareToPlay (double initSampleRate, int samplesPerBlock)
{
    sampleRate = initSampleRate;
}

void FreezeGranulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FreezeGranulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FreezeGranulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
        {
            myBPM = currentPositionInfo.bpm;
        }
        if (currentPositionInfo.isPlaying != isPlaying)
        {
            isPlaying = currentPositionInfo.isPlaying;
            if (isPlaying == false)
            {
                time = 0;
                timeTranspose = -1;
                timeDensity = -1;
                timePosition = -1;
                timeLength = -1;
                timePanning = -1;
                timeLevel = -1;
            }
        }
        if (currentPositionInfo.ppqPosition != ppqPosition)
        {
            ppqPosition = currentPositionInfo.ppqPosition;
        }
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
bool FreezeGranulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FreezeGranulatorAudioProcessor::createEditor()
{
    return new FreezeGranulatorAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void FreezeGranulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FreezeGranulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
void FreezeGranulatorAudioProcessor::updateFile()
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

void FreezeGranulatorAudioProcessor::run()
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

            // Get grain direction
            int grainDirectionId = -1;
            i = 0;
            do
            {
                float direction_i = *(parameters.getRawParameterValue(directionsId[i]));
                if (direction_i > 0.5f) grainDirectionId = i;
                ++i;
            } while (i < directionsId.size() && grainDirectionId < 0);
            if (grainDirectionId == 2) grainDirectionId = random.nextInt(2);

            // Compute transpose grain parameter according to random and freeze values
            if (*(parameters.getRawParameterValue("freezeTransposeOff")) > 0.5f || time > timeTranspose)
            {
                float transpose = *(parameters.getRawParameterValue("transpose"));
                float randTranspose = *(parameters.getRawParameterValue("randTranspose"));
                grainTranspose = transpose + (2.0f * (0.5f - random.nextFloat()) * randTranspose / 100.0f) * 2400.0f;
                grainTranspose = juce::jmin(2400.0f, juce::jmax(-2400.0f, grainTranspose));
            }
            if (time > timeTranspose)
            {
                if (*(parameters.getRawParameterValue("freezeTransposeNote")) > 0.5f && myBPM > 0)
                {
                    float freezeTransposeSynched = tempoScales[int(*(parameters.getRawParameterValue("freezeTransposeSynched")))];
                    timeTranspose = time + int(freezeTransposeSynched * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("freezeTransposeHz")) > 0.5f)
                {
                    float freezeTranspose = *(parameters.getRawParameterValue("freezeTranspose"));
                    timeTranspose = time + int(1 / freezeTranspose * sampleRate);
                }
            }

            // Compute level grain parameter according to random and freeze values
            if (*(parameters.getRawParameterValue("freezeLevelOff")) > 0.5f || time > timeLevel)
            {
                float level = *(parameters.getRawParameterValue("level"));
                float randLevel = *(parameters.getRawParameterValue("randLevel"));
                grainLevel = level / 100.0f + (2.0f * (0.5f - random.nextFloat()) * randLevel / 100.0f);
                grainLevel = juce::jmin(1.0f, juce::jmax(0.0f, grainLevel));
            }
            if (time > timeLevel)
            {
                if (*(parameters.getRawParameterValue("freezeLevelNote")) > 0.5f && myBPM > 0)
                {
                    float freezeLevelSynched = tempoScales[int(*(parameters.getRawParameterValue("freezeLevelSynched")))];
                    timeLevel = time + int(freezeLevelSynched * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("freezeLevelHz")) > 0.5f)
                {
                    float freezeLevel = *(parameters.getRawParameterValue("freezeLevel"));
                    timeLevel = time + int(1 / freezeLevel * sampleRate);
                }
            }

            // Compute position grain parameter according to random and freeze values
            if (*(parameters.getRawParameterValue("freezePositionOff")) > 0.5f || time > timePosition)
            {
                float position = *(parameters.getRawParameterValue("position"));
                float randPosition = *(parameters.getRawParameterValue("randPosition"));
                grainPosition = position + (2.0f * (0.5f - random.nextFloat()) * randPosition / 100.0f);
                grainPosition = juce::jmin(1.0f, juce::jmax(0.0f, grainPosition));
            }
            if (time > timePosition)
            {
                if (*(parameters.getRawParameterValue("freezePositionNote")) > 0.5f && myBPM > 0)
                {
                    float freezePositionSynched = tempoScales[int(*(parameters.getRawParameterValue("freezePositionSynched")))];
                    timePosition = time + int(freezePositionSynched * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("freezePositionHz")) > 0.5f)
                {
                    float freezePosition = *(parameters.getRawParameterValue("freezePosition"));
                    timePosition = time + int(1 / freezePosition * sampleRate);
                }
            }

            // Compute length grain parameter according to random and freeze values
            if (*(parameters.getRawParameterValue("freezeLengthOff")) > 0.5f || time > timeLength)
            {
                float length = *(parameters.getRawParameterValue("length"));
                float randLength = *(parameters.getRawParameterValue("randLength"));
                grainLength = length + (2.0f * (0.5f - random.nextFloat()) * (float)pow(randLength / 100.0, 1.0 / 0.3)) * 10000.0f;
                grainLength = juce::jmin(10000.0f, juce::jmax(1.0f, grainLength));
            }
            if (time > timeLength)
            {
                if (*(parameters.getRawParameterValue("freezeLengthNote")) > 0.5f && myBPM > 0)
                {
                    float freezeLengthSynched = tempoScales[int(*(parameters.getRawParameterValue("freezeLengthSynched")))];
                    timeLength = time + int(freezeLengthSynched * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("freezeLengthHz")) > 0.5f)
                {
                    float freezeLength = *(parameters.getRawParameterValue("freezeLength"));
                    timeLength = time + int(1 / freezeLength * sampleRate);
                }
            }

            // Compute density grain parameter according to random and freeze values
            if (*(parameters.getRawParameterValue("freezeDensityOff")) > 0.5f || time > timeDensity)
            {
                float density = *(parameters.getRawParameterValue("density"));
                float randDensity = *(parameters.getRawParameterValue("randDensity"));
                grainDensity = density + (2.0f * (0.5f - random.nextFloat()) * (float)pow(randDensity / 100.0, 1.0 / 0.4)) * 100.0f;
                grainDensity = juce::jmin(100.0f, juce::jmax(1.0f, grainDensity));
            }
            if (time > timeDensity)
            {
                if (*(parameters.getRawParameterValue("freezeDensityNote")) > 0.5f && myBPM > 0)
                {
                    float freezeDensitySynched = tempoScales[int(*(parameters.getRawParameterValue("freezeDensitySynched")))];
                    timeDensity = time + int(freezeDensitySynched * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("freezeDensityHz")) > 0.5f)
                {
                    float freezeDensity = *(parameters.getRawParameterValue("freezeDensity"));
                    timeDensity = time + int(1 / freezeDensity * sampleRate);
                }
            }

            // Compute panning grain parameter according to random and freeze values
            if (*(parameters.getRawParameterValue("freezePanningOff")) > 0.5f || time > timePanning)
            {
                float panning = *(parameters.getRawParameterValue("panning"));
                float randPanning = *(parameters.getRawParameterValue("randPanning"));
                grainPanning = panning + (2.0f * (0.5f - random.nextFloat()) * randPanning);
                grainPanning = juce::jmin(100.0f, juce::jmax(-100.0f, grainPanning));
            }
            if (time > timePanning)
            {
                if (*(parameters.getRawParameterValue("freezePanningNote")) > 0.5f && myBPM > 0)
                {
                    float freezePanningSynched = tempoScales[int(*(parameters.getRawParameterValue("freezePanningSynched")))];
                    timePanning = time + int(freezePanningSynched * sampleRate / (myBPM / 60));
                }
                if (*(parameters.getRawParameterValue("freezePanningHz")) > 0.5f)
                {
                    float freezePanning = *(parameters.getRawParameterValue("freezePanning"));
                    timePanning = time + int(1 / freezePanning * sampleRate);
                }
            }

            // Convert some parameters from time to number of samples
            float grainRate = pow(10, grainTranspose / (1200.0 * 3.322038403));
            int grainLengthInSamples = int(grainLength / 1000.0 * sampleRate);

            // Compute onset time for the grain
            long long int onset = time + delayOnset;

            // Generate grain and add it to the array of grains
            Grain grain = *new Grain(onset, grainLengthInSamples, grainLevel, grainPosition, grainPanning, grainRate, grainEnvelopeId, grainDirectionId);
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

void FreezeGranulatorAudioProcessor::checkForBuffersToFree()
{
    for (auto i = buffers.size(); --i >= 0;)                           // [1]
    {
        ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i)); // [2]

        if (buffer->getReferenceCount() == 2)                          // [3]
            buffers.remove(i);
    }
}

void FreezeGranulatorAudioProcessor::clearBuffer()
{
    const juce::SpinLock::ScopedLockType lock(mutex);
    currentBuffer = nullptr;
}

void FreezeGranulatorAudioProcessor::processMidi(juce::MidiBuffer& midiMessages)
{
    juce::MidiBuffer::Iterator iterator(midiMessages);
    juce::MidiMessage message;
    int time;

    while (iterator.getNextEvent(message, time))
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
    return new FreezeGranulatorAudioProcessor();
}
