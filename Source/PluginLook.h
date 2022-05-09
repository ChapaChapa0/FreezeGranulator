/*
  ==============================================================================

    PluginLook.h
    Created: 4 Apr 2022 4:17:19pm
    Author:  Chapa

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class PluginLook : public juce::LookAndFeel_V4
{
public:
    PluginLook()
    {
        setColour(juce::Slider::thumbColourId, blazeorange);
        setColour(juce::Label::textColourId, juce::Colours::black);

        setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    }

    juce::Colour coolblue = juce::Colour(108, 142, 191); //coolblue : #6C8EBF
    juce::Colour blazeorange = juce::Colour(255, 130, 0); // blaze orange : #FF8200
};
