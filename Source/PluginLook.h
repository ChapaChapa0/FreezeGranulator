/*
  ==============================================================================

    PluginLook.h
    Created: 4 Apr 2022 4:17:19pm
    Author:  HUAWEI

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class PluginLook : public juce::LookAndFeel_V4
{
public:
    PluginLook();

    juce::Colour blazeorange = juce::Colour(255, 130, 0); // blaze orange : #FF8200
};
