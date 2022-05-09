/*
  ==============================================================================

    Author:  Chapa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Grain.h"
#include "ReferenceCountedBuffer.h"

//==============================================================================
/**
*/
class ChapaGranulatorAudioProcessor  : public juce::AudioProcessor, private juce::Thread
{
public:
    //==============================================================================
    ChapaGranulatorAudioProcessor();
    ~ChapaGranulatorAudioProcessor() override;

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
    void updateValue();
    void updateFile();
    
    juce::MidiKeyboardState keyboardState;
    juce::String filePath;
    juce::AudioFormatManager formatManager;

    // CurrentPositionInfo object used to get BPM from host
    juce::AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    float myBPM = -1.0f;

    double sampleRate;
    long long int time;

    int maxGrains = 200;
    int delayOnset = 500;
    bool play = false;

    float grainLength, grainPosition, grainTranspose, grainDensity, grainLevel, grainPanning;
    long long int timeLength, timePosition, timeTranspose, timeDensity, timeLevel, timePanning;

    juce::StringArray envelopeId = juce::StringArray("envelopeSine", "envelopeTriangle", "envelopeRectangle", "envelopeRampUp", "envelopeRampDown", "envelopeRandom");
    juce::StringArray inertiaId = juce::StringArray("inertiaTransposeOff", "inertiaTransposeNote", "inertiaTransposeHz",
                                                    "inertiaDensityOff", "inertiaDensityNote", "inertiaDensityHz",
                                                    "inertiaPositionOff", "inertiaPositionNote", "inertiaPositionHz",
                                                    "inertiaLengthOff", "inertiaLengthNote", "inertiaLengthHz",
                                                    "inertiaPanningOff", "inertiaPanningNote", "inertiaPanningHz",
                                                    "inertiaLevelOff", "inertiaLevelNote", "inertiaLevelHz");
    juce::StringArray slidersId = juce::StringArray("transpose", "density", "position", "length", "panning", "level", 
                                                    "randTranspose", "randDensity", "randPosition", "randLength", "randPanning", "randLevel", 
                                                    "inertiaTranspose", "inertiaDensity", "inertiaPosition", "inertiaLength", "inertiaPanning", "inertiaLevel");

private:
    juce::Random random;

    void run() override;
    void clearBuffer();
    void checkForBuffersToFree();

    juce::Array<Grain> grainArray;

    juce::SpinLock mutex;
    juce::ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr currentBuffer;

    juce::AudioProcessorValueTreeState parameters;    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChapaGranulatorAudioProcessor)
};
