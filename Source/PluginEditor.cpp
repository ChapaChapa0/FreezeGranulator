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
      thumbnailCache(5), thumbnail(512, audioProcessor.formatManager, thumbnailCache),
      thumbnailTarget(p, thumbnail)
{
    setLookAndFeel(&chapaGranulatorLook);
    //setResizable(true, true);

    thumbnail.addChangeListener(this);

    // Set parameters bounds, id and name
    juce::StringArray slidersText = juce::StringArray("transpose", "density", "position", "length", "panning", "level",
                                                      "rand trans", "rand dens", "rand pos", "rand length", "rand pan", "rand level",
                                                      "freeze trans", "freeze dens", "freeze pos", "freeze length", "freeze pan", "freeze level");
    juce::StringArray slidersSuffix = juce::StringArray(" cts", "x", "x", " ms", "x", "x", "%", "%", "%", "%", "%", "%", " Hz", " Hz", " Hz", " Hz", " Hz", " Hz");

    // Set parameters sliders
    for (int i = 0; i < audioProcessor.slidersId.size(); ++i)
    {
        parameterSliderAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, audioProcessor.slidersId[i], parameterSliders[i]));
        parameterSliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        parameterLabels[i].setText(slidersText[i], juce::NotificationType::dontSendNotification);
        parameterLabels[i].attachToComponent(&(parameterSliders[i]), false);
        parameterSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        parameterSliders[i].setTextBoxIsEditable(true);
        parameterSliders[i].setTextValueSuffix(slidersSuffix[i]);
        parameterSliders[i].setColour(juce::Slider::textBoxOutlineColourId, chapaGranulatorLook.coolblue);
        parameterSliders[i].addListener(this);
        parameterSliders[i].setBounds(300 + (i % 6) * 109, 40 + int(i / 6) * 140 + 300, 100, 100);
        addAndMakeVisible(&parameterSliders[i]);
    }
    for (int i = 0; i < audioProcessor.synchedSlidersId.size(); ++i)
    {
        synchedSliderAttachments[i].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, audioProcessor.synchedSlidersId[i], synchedSliders[i]));
        synchedSliders[i].setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        synchedLabels[i].setText(slidersText[12 + i], juce::NotificationType::dontSendNotification);
        synchedLabels[i].attachToComponent(&(synchedSliders[i]), false);
        synchedSliders[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        synchedSliders[i].setTextBoxIsEditable(false);
        synchedSliders[i].setColour(juce::Slider::textBoxOutlineColourId, chapaGranulatorLook.coolblue);
        synchedSliders[i].addListener(this);
        synchedSliders[i].setBounds(300 + (i % 6) * 109, 320 + 300, 100, 100);
        addChildComponent(&synchedSliders[i]);
    }

    // Set envelope buttons
    for (int i = 0; i < audioProcessor.envelopesId.size(); ++i)
    {
        envelopeButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, audioProcessor.envelopesId[i], envelopeButtons[i]));
        envelopeButtons[i].setButton(i);
        envelopeButtons[i].setClickingTogglesState(true);
        envelopeButtons[i].setRadioGroupId(100, juce::NotificationType::sendNotification);
        envelopeButtons[i].addListener(this);
        envelopeButtons[i].setBounds(20 + (i % 3) * 70, 180 + int(i / 3) * 70 + 300, 60, 60);
        addAndMakeVisible(&envelopeButtons[i]);
    }
    envelopeLabel.setText("Envelope", juce::NotificationType::dontSendNotification);
    envelopeLabel.attachToComponent(&(envelopeButtons[0]), false);

    // Set freeze buttons
    for (int i = 0; i < audioProcessor.freezesId.size(); ++i)
    {
        freezeButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, audioProcessor.freezesId[i], freezeButtons[i]));
        freezeButtons[i].setButton(i % 3);
        freezeButtons[i].setClickingTogglesState(true);
        freezeButtons[i].setRadioGroupId(200 + int(i / 3), juce::NotificationType::sendNotification);
        freezeButtons[i].addListener(this);
        freezeButtons[i].setBounds(300 + i * 33 + int(i / 3) * 10, 425 + 300, 30, 30);
        addAndMakeVisible(&freezeButtons[i]);
    }

    // Set freeze sliders visibility
    for (int i = 0; i < audioProcessor.freezesId.size(); ++i)
    {
        bool synchedBool = freezeButtons[i].getToggleState();
        int numParameterSlider = 12 + int(i / 3);
        int numSynchedSlider = int(i / 3);
        if (i % 3 == 1 && synchedBool)
        {
            parameterSliders[numParameterSlider].setVisible(false);
            synchedSliders[numSynchedSlider].setVisible(true);
        }
    }

    // Set direction buttons
    for (int i = 0; i < audioProcessor.directionsId.size(); ++i)
    {
        directionButtonAttachments[i].reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, audioProcessor.directionsId[i], directionButtons[i]));
        directionButtons[i].setButton(i);
        directionButtons[i].setClickingTogglesState(true);
        directionButtons[i].setRadioGroupId(300, juce::NotificationType::sendNotification);
        directionButtons[i].addListener(this);
        directionButtons[i].setBounds(20 + i * 70, 370 + 300, 60, 60);
        addAndMakeVisible(&directionButtons[i]);
    }
    directionLabel.setText("Direction", juce::NotificationType::dontSendNotification);
    directionLabel.attachToComponent(&(directionButtons[0]), false);

    // Set max grains slider
    maxGrainsAttachments.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "maxGrains", maxGrainsSlider));
    maxGrainsSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    maxGrainsLabel.setText("Max Grains", juce::NotificationType::dontSendNotification);
    maxGrainsLabel.attachToComponent(&(maxGrainsSlider), false);
    maxGrainsSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
    maxGrainsSlider.setTextBoxIsEditable(true);
    maxGrainsSlider.setTextValueSuffix("x");
    maxGrainsSlider.setColour(juce::Slider::textBoxOutlineColourId, chapaGranulatorLook.coolblue);
    maxGrainsSlider.addListener(this);
    maxGrainsSlider.setBounds(20, 40 + 300, 200, 100);
    addAndMakeVisible(&maxGrainsSlider);

    // Set open file button
    openButton.setButtonText("Open...");
    openButton.addListener(this);
    openButton.setBounds(10, 280, 980, 20);
    addAndMakeVisible(&openButton);

    // Set the File Drag And Drop Target
    thumbnailTarget.setBounds(10, 20, 980, 250);
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

    juce::Rectangle<int> thumbnailBounds(10, 20, getWidth() - 20, 250);

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
}

void ChapaGranulatorAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    button->repaint();
    if (button == &openButton) openButtonClicked();

    else
    {
        int i = 0;
        bool freezeButtonClicked = false;
        do
        {
            if (button == &freezeButtons[i])
            {
                showFreezeSlider(i);
                freezeButtonClicked = true;
            }
            ++i;
        } while (i < audioProcessor.freezesId.size() && freezeButtonClicked == false);

    }

}

void ChapaGranulatorAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
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
}


void ChapaGranulatorAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail) thumbnailChanged();
}

//==============================================================================

void ChapaGranulatorAudioProcessorEditor::showFreezeSlider(int index)
{
    int numParameterSlider = 12 + int(index / 3);
    int numSynchedSlider = int(index / 3);
    if (index % 3 == 1)
    {
        parameterSliders[numParameterSlider].setVisible(false);
        synchedSliders[numSynchedSlider].setVisible(true);
    }
    else
    {
        parameterSliders[numParameterSlider].setVisible(true);
        synchedSliders[numSynchedSlider].setVisible(false);
    }
}