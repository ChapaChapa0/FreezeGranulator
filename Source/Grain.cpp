/*
  ==============================================================================

    Grain.cpp
    Created: 23 Mar 2022 1:45:55pm
    Author:  HUAWEI

  ==============================================================================
*/

#include "Grain.h"

Grain::Grain(int onset, int length, int startPos) : onset(onset), length(length), startPosition(startPos)
{
}

Grain::Grain()
{
    onset = 0;
    length = 0;
    startPosition = 0;
}

Grain::~Grain()
{
}

void Grain::process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time)
{
    int fileBufferNumChannels = fileBuffer.getNumChannels();

    if (fileNumSamples == 0 || fileBufferNumChannels == 0) return;

    for (int channel = 0; channel < numChannels; ++channel) 
    {
        currentBlock.addSample(channel, time % blockNumSamples, fileBuffer.getSample(channel % fileBufferNumChannels, time % fileNumSamples));
    }
}

void Grain::updateValue()
{
    for (int i = 0; i < 4; ++i)
    {
        auto parameterEnvelope = params->getRawParameterValue(strs[i]);
        if (*parameterEnvelope > 0.5f) envelopeId = i;
    }
}
