/*
  ==============================================================================

    Grain.h
    Created: 23 Mar 2022 1:45:55pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Grain
{
public:
    Grain(long long int onset, int length, float level, float startPos, float panning, float transpose, int envelopeId);
    Grain();
    ~Grain();

    float envelope(int time);
    void process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time);

    long long int onset;
    int length, envelopeId;
    float level, startPosition, panning, rate;

private:
    float hannWindow(int n, int N);
    float hammingWindow(int n, int N);
    float triangularWindow(int n, int N);
    float rectangularWindow(int n, int N);
    float rampUpWindow(int n, int N);
    float rampDownWindow(int n, int N);

    double levelMidiNote = 0.0;
};