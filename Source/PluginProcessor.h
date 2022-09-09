/*
  ==============================================================================

    Author:  Chapa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Grain.h"
#include "ReferenceCountedBuffer.h"

#define LOG(textToWrite)    JUCE_BLOCK_WITH_FORCED_SEMICOLON (juce::String tempDbgBuf; tempDbgBuf << textToWrite; juce::Logger::writeToLog (tempDbgBuf);)

//==============================================================================
/**
*/
class FreezeGranulatorAudioProcessor  : public juce::AudioProcessor, private juce::Thread
{
public:
    //==============================================================================
    FreezeGranulatorAudioProcessor();
    ~FreezeGranulatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void updateFile();
    
    juce::MidiKeyboardState keyboardState;
    juce::String filePath;
    juce::AudioFormatManager formatManager;

    // CurrentPositionInfo object used to get BPM from host
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    float myBPM = -1.0f;
    bool isPlaying = false;
    double ppqPosition = 0.0;

    int midiNotes[128] = { 0 };

    double sampleRate;
    long long int time;

    // Delay before activation of each grain
    int delayOnset = 80;

    float grainLength, grainPosition, grainTranspose, grainDensity, grainLevel, grainPanning;
    long long int timeLength, timePosition, timeTranspose, timeDensity, timeLevel, timePanning;

    // Set the id for all parameters
    juce::StringArray envelopesId = juce::StringArray("envelopeSine", "envelopeTriangle", "envelopeRectangle", "envelopeRampUp", "envelopeRampDown", "envelopeRandom");
    juce::StringArray freezesId = juce::StringArray("freezeTransposeOff", "freezeTransposeNote", "freezeTransposeHz",
                                                    "freezeDensityOff", "freezeDensityNote", "freezeDensityHz",
                                                    "freezePositionOff", "freezePositionNote", "freezePositionHz",
                                                    "freezeLengthOff", "freezeLengthNote", "freezeLengthHz",
                                                    "freezePanningOff", "freezePanningNote", "freezePanningHz",
                                                    "freezeLevelOff", "freezeLevelNote", "freezeLevelHz");
    juce::StringArray slidersId = juce::StringArray("transpose", "density", "position", "length", "panning", "level", 
                                                    "randTranspose", "randDensity", "randPosition", "randLength", "randPanning", "randLevel", 
                                                    "freezeTranspose", "freezeDensity", "freezePosition", "freezeLength", "freezePanning", "freezeLevel");
    juce::StringArray synchedSlidersId = juce::StringArray("freezeTransposeSynched", "freezeDensitySynched", "freezePositionSynched", 
                                                           "freezeLengthSynched", "freezePanningSynched", "freezeLevelSynched");
    juce::StringArray directionsId = juce::StringArray("directionForward", "directionBackward", "directionRandom");

    // Store all synched values possible
    float tempoScales[10] = { 0.015625f, 0.03125f, 0.0625f, 0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f };

    // Create log file
    juce::FileLogger* fileLog = juce::FileLogger::createDefaultAppLogger("FreezeGranulator", "FreezeGranulator.log", "FreezeGranulator LOG:", 256 * 1024);

private:
    juce::Random random;

    void run() override;
    void clearBuffer();
    void checkForBuffersToFree();
    void processMidi(juce::MidiBuffer&);

    // Array contening all active grains, when a grain has finished it is removed from the array
    juce::Array<Grain> grainArray;

    juce::SpinLock mutex;
    juce::ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr currentBuffer;

    // Value tree state contening the values of all parameters
    juce::AudioProcessorValueTreeState parameters;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreezeGranulatorAudioProcessor)
};
