/*
  ==============================================================================

    EnvelopeButton.h
    Created: 28 Feb 2022 5:59:41pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopeButton : public juce::ImageButton
{
public:
    EnvelopeButton() {}

    void setEnvelope(int indexEnvelope)
    {
        setImages(true, true, true, imageOff[indexEnvelope], 1.0f, juce::Colours::transparentWhite, juce::Image(), 1.0f,
            juce::Colours::transparentWhite, imageOn[indexEnvelope], 1.0f, juce::Colours::transparentWhite, 0.0f);
    }

    juce::Image imageOn[6] =
    {
        juce::ImageCache::getFromMemory(BinaryData::env_sine_on_png, BinaryData::env_sine_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_triangle_on_png, BinaryData::env_triangle_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_rectangle_on_png, BinaryData::env_rectangle_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_rampup_on_png, BinaryData::env_rampup_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_rampdown_on_png, BinaryData::env_rampdown_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_random_on_png, BinaryData::env_random_on_pngSize)
    };

    juce::Image imageOff[6] =
    {
        juce::ImageCache::getFromMemory(BinaryData::env_sine_off_png, BinaryData::env_sine_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_triangle_off_png, BinaryData::env_triangle_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_rectangle_off_png, BinaryData::env_rectangle_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_rampup_off_png, BinaryData::env_rampup_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_rampdown_off_png, BinaryData::env_rampdown_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::env_random_off_png, BinaryData::env_random_off_pngSize)
    };

private:
    int indexEnvelope = 0;
};