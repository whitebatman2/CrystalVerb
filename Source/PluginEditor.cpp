//
//  PluginEditor.cpp
//  Miłosz Kolenderski
//

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FdnConfig.hpp"


using namespace Parameters;

//==============================================================================
ReverbPluginAudioProcessorEditor::ReverbPluginAudioProcessorEditor (ReverbPluginAudioProcessor& p)
    :
AudioProcessorEditor (&p),
audioProcessor (p),
mToggles
{
    AttachedToggle(audioProcessor, PID::Bypass),
    AttachedToggle(audioProcessor, PID::DiffusionEnable)
},
mFeedbackType(audioProcessor, PID::FeedbackType),
mRoomSizeMinSlider(audioProcessor, PID::RoomSizeMin),
mRoomSizeMaxSlider(audioProcessor, PID::RoomSizeMax),
mDiffusionPane(audioProcessor),
mStaticFdnGainsPane(audioProcessor),
mFdnPane(audioProcessor),
mOutEqualizerPane(audioProcessor),
mOutKnobPane(audioProcessor),
mRoomSizePane(audioProcessor)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(mDiffusionPane);
    addAndMakeVisible(mStaticFdnGainsPane);
    addAndMakeVisible(mFdnPane);
    addAndMakeVisible(mOutEqualizerPane);
    addAndMakeVisible(mOutKnobPane);
    addAndMakeVisible(mRoomSizePane);

    setSize (900, 700);
}

ReverbPluginAudioProcessorEditor::~ReverbPluginAudioProcessorEditor()
{
}

//==============================================================================
void ReverbPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ReverbPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto topBar = bounds.removeFromTop(280);

    auto topLeft = topBar.removeFromLeft(270);
    mDiffusionPane.setBounds(topLeft.removeFromTop(130));
    mRoomSizePane.setBounds(topLeft);

    mStaticFdnGainsPane.setBounds(topBar.removeFromRight(630));
    mOutKnobPane.setBounds(bounds.removeFromRight(120));

    mFdnPane.setBounds(bounds.removeFromLeft(350));
    mOutEqualizerPane.setBounds(bounds);
}
