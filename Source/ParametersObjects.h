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

    void setButton(int index)
    {
        setImages(true, true, true, imageOff[index], 1.0f, juce::Colours::transparentWhite, juce::Image(), 1.0f,
            juce::Colours::transparentWhite, imageOn[index], 1.0f, juce::Colours::transparentWhite, 0.0f);
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

//==============================================================================

class FreezeButton : public juce::ImageButton
{
public:
    FreezeButton() {}

    void setButton(int index)
    {
        setImages(true, true, true, imageOff[index], 1.0f, juce::Colours::transparentWhite, juce::Image(), 1.0f,
            juce::Colours::transparentWhite, imageOn[index], 1.0f, juce::Colours::transparentWhite, 0.0f);
    }

    juce::Image imageOn[3] =
    {
        juce::ImageCache::getFromMemory(BinaryData::inertia_off_on_png, BinaryData::inertia_off_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::inertia_note_on_png, BinaryData::inertia_note_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::inertia_hz_on_png, BinaryData::inertia_hz_on_pngSize)
    };

    juce::Image imageOff[3] =
    {
        juce::ImageCache::getFromMemory(BinaryData::inertia_off_off_png, BinaryData::inertia_off_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::inertia_note_off_png, BinaryData::inertia_note_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::inertia_hz_off_png, BinaryData::inertia_hz_off_pngSize)
    };

private:
    int indexEnvelope = 0;
};

//==============================================================================

class DirectionButton : public juce::ImageButton
{
public:
    DirectionButton() {}

    void setButton(int index)
    {
        setImages(true, true, true, imageOff[index], 1.0f, juce::Colours::transparentWhite, juce::Image(), 1.0f,
            juce::Colours::transparentWhite, imageOn[index], 1.0f, juce::Colours::transparentWhite, 0.0f);
    }

    juce::Image imageOn[3] =
    {
        juce::ImageCache::getFromMemory(BinaryData::direction_forward_on_png, BinaryData::direction_forward_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::direction_backward_on_png, BinaryData::direction_backward_on_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::direction_random_on_png, BinaryData::direction_random_on_pngSize)
    };

    juce::Image imageOff[3] =
    {
        juce::ImageCache::getFromMemory(BinaryData::direction_forward_off_png, BinaryData::direction_forward_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::direction_backward_off_png, BinaryData::direction_backward_off_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::direction_random_off_png, BinaryData::direction_random_off_pngSize)
    };

private:
    int indexEnvelope = 0;
};

//==============================================================================

class SynchedSlider : public juce::Slider
{

public:
    SynchedSlider() {}
    ~SynchedSlider() {}

    virtual juce::String getTextFromValue(double value) override
    {
        return tempoStrings[int(value)];
    }

    juce::StringArray tempoStrings = juce::StringArray("1/64", "1/32", "1/16", "1/8", "1/4", "1/2", "1 bar", "2 bars", "4 bars", "8 bars");
};

//==============================================================================

class ThumbnailFileDragAndDropTarget : public juce::Component, public juce::FileDragAndDropTarget
{

public:
    ThumbnailFileDragAndDropTarget(FreezeGranulatorAudioProcessor& p, juce::AudioThumbnail& thumbnail)
        : audioProcessor(p), audioThumbnail(thumbnail) {}

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::transparentWhite);
    }

    bool isInterestedInFileDrag(const juce::StringArray& /*files*/) override
    {
        return true;
    }

    void fileDragEnter(const juce::StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
        repaint();
    }

    void fileDragMove(const juce::StringArray& /*files*/, int /*x*/, int /*y*/) override
    {
    }

    void fileDragExit(const juce::StringArray& /*files*/) override
    {
        repaint();
    }

    void filesDropped(const juce::StringArray& files, int /*x*/, int /*y*/) override
    {
        auto path = files.begin();
        auto file = juce::File(*path);

        if (file != juce::File{})
        {
            audioProcessor.filePath = *path;
            audioThumbnail.setSource(new juce::FileInputSource(file));
            audioProcessor.updateFile();
        }
        repaint();
    }

    juce::AudioThumbnail& audioThumbnail;
    FreezeGranulatorAudioProcessor& audioProcessor;
};