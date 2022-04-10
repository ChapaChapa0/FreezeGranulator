/*
  ==============================================================================

    Grain.h
    Created: 23 Mar 2022 1:45:55pm
    Author:  HUAWEI

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Grain
{
public:
    Grain(int onset, int length, int startPos, juce::AudioProcessorValueTreeState& args);
    Grain(juce::AudioProcessorValueTreeState& args);
    Grain();
    ~Grain();

    void envelope(int time);
    void process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time);
    void updateValue();
    void initEnvelopeValues();

private:
    int onset;
    int length;
    int startPosition;

    double amp, rdur, rdur2, slope, curve;

    juce::StringRef strs[8] =
    {
        juce::StringRef("level"),
        juce::StringRef("length"),
        juce::StringRef("randLevel"),
        juce::StringRef("randLength"),

        juce::StringRef("envelope1"),
        juce::StringRef("envelope2"),
        juce::StringRef("envelope3"),
        juce::StringRef("envelope4"),
    };

    float paramAmplitude, paramLength, paramAmplitudeR, paramLengthR;
    int envelopeId;

    double levelMidiNote = 0.0;
    juce::AudioProcessorValueTreeState* params;
};