/*
  ==============================================================================

    Author:  Chapa

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChapaGranulatorAudioProcessorEditor::ChapaGranulatorAudioProcessorEditor (ChapaGranulatorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts), 
      keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      thumbnailCache(5), thumbnail(512, formatManager, thumbnailCache)
{
    setLookAndFeel(&chapaGranulatorLook);

    openButton.setButtonText("Open...");
    openButton.addListener(this);
    openButton.setBounds(10, 300, 780, 20);
    addAndMakeVisible(&openButton);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    thumbnail.addChangeListener(this);

    // Set parameters bounds, id and name
    auto buttonsId = juce::StringArray("envelope1", "envelope2", "envelope3", "envelope4");
    //auto buttonBounds = ((10, 10, 50, 50), (70, 10, 50, 50), (10, 70, 50, 50), (70, 70, 50, 50));

    auto slidersId = juce::StringArray("tune", "fine", "density", "position", "length", "level", "randTune",  "randDensity", "randPosition", "randLength", "randLevel");
    auto slidersText = juce::StringArray("tune", "fine", "density", "position", "length", "level", "r tune", "r density", "r position", "r length", "r level");
    auto slidersSuffix = juce::StringArray(" sts", " cents", "%", "", " ms", "%", "%", "%", "%", "%", "%");
    //auto slidersBounds = ((10, 10, 50, 50), (70, 10, 50, 50), (10, 70, 50, 50), (70, 70, 50, 50));

    // Set the envelope buttons
    for (int i = 0; i < buttonsId.size(); ++i)
    {
        envelopeButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, buttonsId[i], envelopeButtons[i]));
        envelopeButtons[i].setEnvelope(i);
        envelopeButtons[i].setClickingTogglesState(true);
        envelopeButtons[i].setRadioGroupId(1, juce::NotificationType::sendNotification);
        envelopeButtons[i].addListener(this);
        envelopeButtons[i].setBounds(10 + (i % 2) * 60, 40 + int(i / 2) * 60, 50, 50);
        addAndMakeVisible(&envelopeButtons[i]);
    }

    for (int i = 0; i < slidersId.size(); ++i)
    {
        sliderAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, slidersId[i], parameterSliders[i]));
        parameterSliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        parameterLabels[i].setText(slidersText[i], juce::NotificationType::dontSendNotification);
        parameterLabels[i].attachToComponent(&(parameterSliders[i]), false);
        parameterSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        parameterSliders[i].setTextBoxIsEditable(false);
        parameterSliders[i].setTextValueSuffix(slidersSuffix[i]);
        parameterSliders[i].addListener(this);
        parameterSliders[i].setBounds(180 + (i % 6) * 100, 40 + int(i / 6) * 140, 100, 100);
        addAndMakeVisible(&parameterSliders[i]);
    }
    
    setSize (800, 600);
}

ChapaGranulatorAudioProcessorEditor::~ChapaGranulatorAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void ChapaGranulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    juce::Rectangle<int> thumbnailBounds(10, 330, getWidth() - 20, 250);

    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, thumbnailBounds);
    else
        paintIfFileLoaded(g, thumbnailBounds);
}

void ChapaGranulatorAudioProcessorEditor::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(juce::Colours::transparentBlack);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::darkorange);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void ChapaGranulatorAudioProcessorEditor::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(juce::Colours::transparentBlack);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::darkorange);

    thumbnail.drawChannel(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 0, 1.0f);
    thumbnail.drawChannel(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1, 1.0f);
}

void ChapaGranulatorAudioProcessorEditor::thumbnailChanged()
{
    repaint();
}

void ChapaGranulatorAudioProcessorEditor::resized()
{
}

//==============================================================================

void ChapaGranulatorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.updateValue();
}

void ChapaGranulatorAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    button->repaint();
    if (button == &openButton) openButtonClicked();
}

void ChapaGranulatorAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
    audioProcessor.updateValue();
}

void ChapaGranulatorAudioProcessorEditor::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                juce::String path(file.getFullPathName());
                audioProcessor.chosenPath = path;

            //    auto* reader = formatManager.createReaderFor(file);

            //    if (reader != nullptr)
            //    {
            //        auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            //        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            //        thumbnail.setSource(new juce::FileInputSource(file));
            //        readerSource.reset(newSource.release());
            //    }
            }
        });

    audioProcessor.updateValue();
}

void ChapaGranulatorAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail) thumbnailChanged();
}