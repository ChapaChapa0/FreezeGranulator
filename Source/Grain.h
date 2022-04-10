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
    Grain(int onset, int length, int startPos);
    Grain();
    ~Grain();

    void process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time);
    void updateValue();

private:
    int onset;
    int length;
    int startPosition;

    juce::StringRef strs[4] =
    {
        juce::StringRef("envelope1"),
        juce::StringRef("envelope2"),
        juce::StringRef("envelope3"),
        juce::StringRef("envelope4"),
    };

    juce::Array<float> envelopeButtons = { 1.0, 0.0, 0.0, 0.0 };
    int envelopeId = 0;

    double levelMidiNote = 0.0, tailOff = 0.0, attackEnvelop = 0.0f;
    juce::AudioProcessorValueTreeState* params;
};