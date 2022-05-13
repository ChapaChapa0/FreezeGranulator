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
      thumbnailCache(5), thumbnail(512, audioProcessor.formatManager, thumbnailCache)
{
    setLookAndFeel(&chapaGranulatorLook);
    //setResizable(true, true);

    thumbnail.addChangeListener(this);

    // Set parameters bounds, id and name
    juce::StringArray slidersText = juce::StringArray("transpose", "density", "position", "length", "panning", "level",
                                                      "r transpose", "r density", "r position", "r length", "r panning", "r level",
                                                      "i transpose", "i density", "i position", "i length", "i panning", "i level");
    juce::StringArray slidersSuffix = juce::StringArray(" cts", "x", "x", " ms", "x", "x", "%", "%", "%", "%", "%", "%", " Hz", " Hz", " Hz", " Hz", " Hz", " Hz");

    // Set parameters sliders
    for (int i = 0; i < audioProcessor.slidersId.size(); ++i)
    {
        sliderAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, audioProcessor.slidersId[i], parameterSliders[i]));
        parameterSliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        parameterLabels[i].setText(slidersText[i], juce::NotificationType::dontSendNotification);
        parameterLabels[i].attachToComponent(&(parameterSliders[i]), false);
        parameterSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        parameterSliders[i].setTextBoxIsEditable(true);
        parameterSliders[i].setTextValueSuffix(slidersSuffix[i]);
        parameterSliders[i].setColour(juce::Slider::textBoxOutlineColourId, chapaGranulatorLook.coolblue);
        parameterSliders[i].addListener(this);
        parameterSliders[i].setBounds(340 + (i % 6) * 109, 40 + int(i / 6) * 140, 100, 100);
        addAndMakeVisible(&parameterSliders[i]);
    }

    // Set envelope buttons
    for (int i = 0; i < audioProcessor.envelopesId.size(); ++i)
    {
        envelopeButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, audioProcessor.envelopesId[i], envelopeButtons[i]));
        envelopeButtons[i].setButton(i);
        envelopeButtons[i].setClickingTogglesState(true);
        envelopeButtons[i].setRadioGroupId(100, juce::NotificationType::sendNotification);
        envelopeButtons[i].addListener(this);
        envelopeButtons[i].setBounds(20 + (i % 3) * 70, 40 + int(i / 3) * 70, 60, 60);
        addAndMakeVisible(&envelopeButtons[i]);
    }

    // Set inertia buttons
    for (int i = 0; i < audioProcessor.inertiasId.size(); ++i)
    {
        inertiaButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, audioProcessor.inertiasId[i], inertiaButtons[i]));
        inertiaButtons[i].setButton(i % 3);
        inertiaButtons[i].setClickingTogglesState(true);
        inertiaButtons[i].setRadioGroupId(200 + int(i / 3), juce::NotificationType::sendNotification);
        inertiaButtons[i].addListener(this);
        inertiaButtons[i].setBounds(340 + i * 33 +int(i / 3) * 10, 425, 30, 30);
        addAndMakeVisible(&inertiaButtons[i]);
    }

    // Set direction buttons
    for (int i = 0; i < audioProcessor.directionsId.size(); ++i)
    {
        directionButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, audioProcessor.directionsId[i], directionButtons[i]));
        directionButtons[i].setButton(i);
        directionButtons[i].setClickingTogglesState(true);
        directionButtons[i].setRadioGroupId(300, juce::NotificationType::sendNotification);
        directionButtons[i].addListener(this);
        directionButtons[i].setBounds(20 + i * 70, 300, 60, 60);
        addAndMakeVisible(&directionButtons[i]);
    }

    // Set open file button
    openButton.setButtonText("Open...");
    openButton.addListener(this);
    openButton.setBounds(10, 480, 980, 20);
    addAndMakeVisible(&openButton);

    // Set the File Drag And Drop Target
    thumbnailTarget.setBounds(10, 520, 980, 250);
    addAndMakeVisible(&thumbnailTarget);
    
    // Set GUI size
    setSize (1000, 800);
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

    juce::Rectangle<int> thumbnailBounds(10, 520, getWidth() - 20, 250);

    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, thumbnailBounds);
    else
        paintIfFileLoaded(g, thumbnailBounds);
}

void ChapaGranulatorAudioProcessorEditor::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(juce::Colours::transparentBlack);
    g.fillRect(thumbnailBounds);
    g.setColour(chapaGranulatorLook.blazeorange);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void ChapaGranulatorAudioProcessorEditor::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour(juce::Colours::transparentBlack);
    g.fillRect(thumbnailBounds);
    g.setColour(chapaGranulatorLook.blazeorange);

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
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...", juce::File{}, "*.wav", "*.aif", "*.aiff");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                juce::String path(file.getFullPathName());
                audioProcessor.filePath = path;
                thumbnail.setSource(new juce::FileInputSource(file));
                audioProcessor.updateFile();
            }
        });

    audioProcessor.updateValue();

    LOG("Hello ??");
}


void ChapaGranulatorAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail) thumbnailChanged();
}

//==============================================================================

void ChapaGranulatorAudioProcessorEditor::dragOperationStarted(const juce::DragAndDropTarget::SourceDetails&)
{
}

void ChapaGranulatorAudioProcessorEditor::dragOperationEnded(const juce::DragAndDropTarget::SourceDetails&)
{
    auto path = thumbnailTarget.dropedFilePath;
    auto file = juce::File(path);

    LOG("Hello");

    if (file != juce::File{})
    {
        audioProcessor.filePath = path;
        thumbnail.setSource(new juce::FileInputSource(file));
        audioProcessor.updateFile();
    }
}
