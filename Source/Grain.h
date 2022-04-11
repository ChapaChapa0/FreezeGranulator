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
    Grain(long long int onset, int length, float level, float startPos, int envelopeId);
    Grain();
    ~Grain();

    float envelope(int time);
    void process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time);
    //void initEnvelopeValues();

    long long int onset;
    int length, envelopeId;
    float level, startPosition;

private:
    float hamming_window(int n, int N);
    float triangular_window(int n, int N);
    float rectangular_window(int n, int N);

    //double amp, rdur, rdur2, slope, curve;

    double levelMidiNote = 0.0;
};