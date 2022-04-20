/*
  ==============================================================================

    Author:  Chapa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ParametersObjects.h"
#include "PluginLook.h"

//==============================================================================
/**
*/
class ChapaGranulatorAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, 
                                             public juce::Slider::Listener, private juce::ChangeListener
{
public:
    ChapaGranulatorAudioProcessorEditor (ChapaGranulatorAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~ChapaGranulatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void buttonClicked(juce::Button* button) override;
    virtual void buttonStateChanged(juce::Button* button) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void thumbnailChanged();
    void openButtonClicked();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChapaGranulatorAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::MidiKeyboardComponent keyboardComponent;

    EnvelopeButton envelopeButtons[6];
    juce::Slider parameterSliders[10];
    juce::Label parameterLabels[10];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> envelopeButtonAttachments[6];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments[10];

    juce::TextButton openButton;
    std::unique_ptr<juce::FileChooser> chooser;

    juce::TextButton playButton;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;

    PluginLook chapaGranulatorLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChapaGranulatorAudioProcessorEditor)
};