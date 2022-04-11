/*
  ==============================================================================

    Grain.cpp
    Created: 23 Mar 2022 1:45:55pm
    Author:  HUAWEI

  ==============================================================================
*/

#include "Grain.h"


Grain::Grain(long long int onset, int length, float level, float startPos, int envId) : onset(onset), length(length), level(level), startPosition(startPos), envelopeId(envId)
{
    //initEnvelopeValues();
}

Grain::Grain()
{
    onset, length, level, startPosition, envelopeId = 0;

    //initEnvelopeValues();
}

Grain::~Grain()
{
}

float Grain::envelope(int time)
{
    float valueEnv;
    int timeEnv = time - onset;
    if (timeEnv > length || timeEnv < 0) valueEnv = 0;

    else
    {
        if (envelopeId == 1) valueEnv = triangular_window(timeEnv, length);
        else if (envelopeId == 2) valueEnv = rectangular_window(timeEnv, length);
        else valueEnv = hamming_window(timeEnv, length);
    }

    return valueEnv;
}

void Grain::process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time)
{
    int fileBufferNumChannels = fileBuffer.getNumChannels();

    if (fileNumSamples == 0 || fileBufferNumChannels == 0) return;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        int actualStartPosition = juce::jmin(int(startPosition * fileNumSamples), int(fileNumSamples - length));
        int currentPosition = juce::jmax(actualStartPosition, (actualStartPosition + time) % fileNumSamples);
        float sample = fileBuffer.getSample(channel % fileBufferNumChannels, currentPosition);

        currentBlock.addSample(channel, time % blockNumSamples, sample * envelope(time) * level);
    }
}

//void Grain::initEnvelopeValues()
//{
//    jassert(length > 0);
//
//    amp = 0.0;
//    rdur = 1.0 / length;
//    rdur2 = rdur * rdur;
//    slope = 4.0 * level * (rdur - rdur2);
//    curve = -8.0 * level * rdur2;
//}

float Grain::hamming_window(int n, int N)
{
    return 0.54 - 0.46 * cos(2.0 * juce::MathConstants<double>::pi * n / N);
}

float Grain::triangular_window(int n, int N)
{
    return (n < N / 2) ? 2.0 * n / N : 1.0 - 2.0 * n / N;
}

float Grain::rectangular_window(int n, int N)
{
    if (n < N / 10) return 2.0 * n / N;
    else if (n > 9 * N / 10) return 1.0 - 2.0 * n / N;
    else return 1.0;
}
