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
}

Grain::Grain()
{
    onset = 0; 
    length = 0;
    envelopeId = 0;
    level = 0.0f;
    startPosition = 0.0f;
}

Grain::~Grain()
{
}

float Grain::envelope(int time)
{
    float valueEnv;
    if (time > length || time < 0) valueEnv = 0;

    else
    {
        if (envelopeId == 1) valueEnv = triangularWindow(time, length);
        else if (envelopeId == 2) valueEnv = rectangularWindow(time, length);
        else if (envelopeId == 3) valueEnv = rampUpWindow(time, length);
        else if (envelopeId == 4) valueEnv = rampDownWindow(time, length);
        else valueEnv = hammingWindow(time, length);
    }

    return valueEnv;
}

void Grain::process(juce::AudioSampleBuffer& currentBlock, juce::AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, int time)
{
    int fileBufferNumChannels = fileBuffer.getNumChannels();

    if (fileNumSamples == 0 || fileBufferNumChannels == 0) return;

    int timeGrain = time - onset;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        int startPositionInSamples = juce::jmax(0, int(startPosition * fileNumSamples - 1));
        int currentPosition = juce::jmax(startPositionInSamples, (startPositionInSamples + timeGrain) % fileNumSamples);
        float sample = fileBuffer.getSample(channel % fileBufferNumChannels, currentPosition);

        currentBlock.addSample(channel, time % blockNumSamples, sample * envelope(timeGrain) * level * 0.2);
    }
}

float Grain::hannWindow(int n, int N)
{
    return 0.5 * (1.0 - cos(2.0 * juce::MathConstants<double>::pi * n / N));
}

float Grain::hammingWindow(int n, int N)
{
    return 0.54 - 0.46 * cos(2.0 * juce::MathConstants<double>::pi * n / N);
}

float Grain::triangularWindow(int n, int N)
{
    return (n < N / 2) ? 2.0 * n / N : 1.0 - 2.0 * (n - N / 2.0) / N;
}

float Grain::rectangularWindow(int n, int N)
{
    double coeff = 10.0;
    if (n < N / coeff) return coeff * n / N;
    else if (n > (coeff - 1) * N / coeff) return 1.0 - coeff * (n - (coeff - 1.0) * N / coeff) / N;
    else return 1.0;
}

float Grain::rampUpWindow(int n, int N)
{
    double coeff = 20.0;
    if (n < (coeff - 1) * N / coeff) return (exp(n * coeff / ((coeff - 1.0) * N)) - 1.0) / (exp(1.0) - 1.0);
    else return 1.0 - coeff * (n - (coeff - 1.0) * N / coeff) / N;
}

float Grain::rampDownWindow(int n, int N)
{
    return rampUpWindow(N - 1 - n, N);
}