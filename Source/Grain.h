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

    void process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, 
                 int numChannels, int blockNumSamples, int fileNumSamples, int time);
    void updateValue();

private:
    int onset;
    int length;
    int startPosition;
};