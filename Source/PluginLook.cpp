/*
  ==============================================================================

    PluginLook.cpp
    Created: 4 Apr 2022 4:17:19pm
    Author:  HUAWEI

  ==============================================================================
*/

#include "PluginLook.h"

PluginLook::PluginLook()
{
    setColour(juce::Slider::thumbColourId, blazeorange);
    setColour(juce::Label::textColourId, juce::Colours::black);

    setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);

}
