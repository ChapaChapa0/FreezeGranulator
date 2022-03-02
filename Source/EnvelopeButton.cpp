/*
  ==============================================================================

    EnvelopeButton.cpp
    Created: 28 Feb 2022 5:59:41pm
    Author:  HUAWEI

  ==============================================================================
*/

#include "EnvelopeButton.h"

EnvelopeButton::EnvelopeButton()
{
}

void EnvelopeButton::setEnvelope(int newEnvelope)
{
    indexEnvelope = newEnvelope;
    setImages(true, true, true, imageOff[indexEnvelope], 1.0f, juce::Colours::transparentWhite, juce::Image(), 1.0f, 
        juce::Colours::transparentWhite, imageOn[indexEnvelope], 1.0f, juce::Colours::transparentWhite, 0.0f);
}