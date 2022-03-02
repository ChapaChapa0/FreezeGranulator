/*
  ==============================================================================

    EnvelopeButton.h
    Created: 28 Feb 2022 5:59:41pm
    Author:  HUAWEI

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopeButton : public juce::ImageButton
{
public:
    EnvelopeButton();
    void setEnvelope(int);

    juce::Image imageOn[4] =
    {
        juce::ImageCache::getFromMemory(BinaryData::WT_sinus_on_png, BinaryData::WT_sinus_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_square_on_png, BinaryData::WT_square_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_triangle_on_png, BinaryData::WT_triangle_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_sawtooth_on_png, BinaryData::WT_sawtooth_on_pngSize)
    };

    juce::Image imageOff[4] =
    {
        juce::ImageCache::getFromMemory(BinaryData::WT_sinus_off_png, BinaryData::WT_sinus_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_square_off_png, BinaryData::WT_square_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_triangle_off_png, BinaryData::WT_triangle_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::WT_sawtooth_off_png, BinaryData::WT_sawtooth_off_pngSize)
    };

private:
    int indexEnvelope = 0;
};