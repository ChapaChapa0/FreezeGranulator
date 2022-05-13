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
                                             public juce::Slider::Listener, private juce::ChangeListener,
                                             public juce::DragAndDropContainer
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

    void dragOperationStarted(const juce::DragAndDropTarget::SourceDetails&) override;
    void dragOperationEnded(const juce::DragAndDropTarget::SourceDetails&) override;

    PluginLook chapaGranulatorLook;

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

    // Buttons and sliders
    EnvelopeButton envelopeButtons[6];
    InertiaButton inertiaButtons[18];
    DirectionButton directionButtons[3];
    juce::Slider parameterSliders[18];
    juce::TextButton openButton;

    // Labels associated to buttons and sliders
    juce::Label envelopeLabel;
    juce::Label directionLabel;
    juce::Label parameterLabels[18];

    // Attachments to buttons and sliders
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> envelopeButtonAttachments[6];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> inertiaButtonAttachments[18];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> directionButtonAttachments[3];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments[18];
    std::unique_ptr<juce::FileChooser> chooser;

    // Audio thumbnail
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    ThumbnailFileDragAndDropTarget thumbnailTarget;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChapaGranulatorAudioProcessorEditor)
};