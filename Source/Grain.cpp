/*
  ==============================================================================

    Grain.cpp
    Created: 23 Mar 2022 1:45:55pm
    Author:  HUAWEI

  ==============================================================================
*/

#include "Grain.h"


Grain::Grain(int onset, int length, int startPos, juce::AudioProcessorValueTreeState& args) : onset(onset), length(length), startPosition(startPos), params(&args)
{
    updateValue();
    initEnvelopeValues();
}

Grain::Grain(juce::AudioProcessorValueTreeState& args) : params(&args)
{
    onset, length, startPosition = 0;

    updateValue();
    initEnvelopeValues();
}

Grain::Grain()
{
    onset, length, startPosition = 0;

    params = nullptr;
    paramAmplitude, paramLength, paramAmplitudeR, paramLengthR = 0.0f;
    amp, rdur, rdur2, slope, curve = 0.0;
    envelopeId = 0;
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

void Grain::updateValue()
{
    paramAmplitude = *(params->getRawParameterValue(strs[0]));
    paramLength = *(params->getRawParameterValue(strs[1]));
    paramAmplitudeR = *(params->getRawParameterValue(strs[2]));
    paramLengthR = *(params->getRawParameterValue(strs[3]));

    for (int i = 0; i < 4; ++i)
    {
        auto grainEnvelope = params->getRawParameterValue(strs[i + 4]);
        if (*grainEnvelope > 0.5f) envelopeId = i;
    }
}

void Grain::initEnvelopeValues()
{
    jassert(paramLength > 0.0f);

    amp = 0.0;
    rdur = 1.0 / paramLength;
    rdur2 = rdur * rdur;
    slope = 4.0 * paramAmplitude * (rdur - rdur2);
    curve = -8.0 * paramAmplitude * rdur2;
}