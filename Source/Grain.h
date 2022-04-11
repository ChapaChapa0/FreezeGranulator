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
    Grain(long long int onset, int length, int amplitude, int startPos, int envelopeId);
    Grain();
    ~Grain();

    void envelope(int time);
    void process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time);
    void initEnvelopeValues();

    long long int onset;
    int length, level, startPosition, envelopeId;

private:
    double amp, rdur, rdur2, slope, curve;

    double levelMidiNote = 0.0;
    juce::AudioProcessorValueTreeState* params;
};