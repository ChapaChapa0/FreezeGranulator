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
class FreezeGranulatorAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, 
                                             public juce::Slider::Listener, private juce::ChangeListener,
                                             public juce::DragAndDropContainer
{
public:
    FreezeGranulatorAudioProcessorEditor (FreezeGranulatorAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~FreezeGranulatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void buttonClicked(juce::Button* button) override;
    virtual void buttonStateChanged(juce::Button* button) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    PluginLook freezeGranulatorLook;

private:
    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void thumbnailChanged();
    void openButtonClicked();

    void showFreezeSlider(int);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FreezeGranulatorAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::MidiKeyboardComponent keyboardComponent;

    // Buttons and sliders
    EnvelopeButton envelopeButtons[6];
    FreezeButton freezeButtons[18];
    DirectionButton directionButtons[3];
    juce::Slider maxGrainsSlider;
    juce::Slider parameterSliders[18];
    SynchedSlider synchedSliders[6];
    juce::TextButton openButton;

    // Labels associated to buttons and sliders
    juce::Label envelopeLabel;
    juce::Label directionLabel;
    juce::Label maxGrainsLabel;
    juce::Label parameterLabels[18];
    juce::Label synchedLabels[6];

    // Attachments to buttons and sliders
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> envelopeButtonAttachments[6];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> freezeButtonAttachments[18];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> directionButtonAttachments[3];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> maxGrainsAttachments;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> parameterSliderAttachments[18];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> synchedSliderAttachments[6];
    std::unique_ptr<juce::FileChooser> chooser;

    // Audio thumbnail
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    ThumbnailFileDragAndDropTarget thumbnailTarget;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreezeGranulatorAudioProcessorEditor)
};