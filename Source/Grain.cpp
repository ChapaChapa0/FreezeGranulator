/*
  ==============================================================================

    Grain.cpp
    Created: 23 Mar 2022 1:45:55pm
    Author:  HUAWEI

  ==============================================================================
*/

#include "Grain.h"


Grain::Grain(long long int onset, int length, int level, int startPos, int envId) : onset(onset), length(length), level(level), startPosition(startPos), envelopeId(envId)
{
    initEnvelopeValues();
}

Grain::Grain()
{
    onset, length, level, startPosition, envelopeId = 0;

    initEnvelopeValues();
}

Grain::~Grain()
{
}

void Grain::envelope(int time)
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

void Grain::initEnvelopeValues()
{
    jassert(length > 0);

    amp = 0.0;
    rdur = 1.0 / length;
    rdur2 = rdur * rdur;
    slope = 4.0 * level * (rdur - rdur2);
    curve = -8.0 * level * rdur2;
}