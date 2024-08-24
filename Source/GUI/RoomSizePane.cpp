//
//  RoomSizePane.cpp
//  Miłosz Kolenderski
//

#include "RoomSizePane.hpp"


using namespace Parameters;

RoomSizePane::RoomSizePane(ReverbPluginAudioProcessor& audioProcessor)
:
mRoomSizeMinSlider(audioProcessor, PID::RoomSizeMin),
mRoomSizeMaxSlider(audioProcessor, PID::RoomSizeMax)
{
    setText("Room size");
    setTextLabelPosition(juce::Justification::centredTop);

    mRoomSizeMinLabel.setText("Min [m]",
                              juce::NotificationType::dontSendNotification);
    mRoomSizeMinLabel.attachToComponent(&mRoomSizeMinSlider, false);

    mRoomSizeMaxLabel.setText("Max [m]",
                              juce::NotificationType::dontSendNotification);
    mRoomSizeMaxLabel.attachToComponent(&mRoomSizeMaxSlider, false);

    addAndMakeVisible(mRoomSizeMinSlider);
    addAndMakeVisible(mRoomSizeMaxSlider);
}

void RoomSizePane::resized()
{
    auto bounds = getLocalBounds();
    auto margin = 12;

    auto itemHeight = 24;
    auto itemWidth = 100;

    bounds.removeFromLeft(margin);
    bounds.removeFromTop(itemHeight);

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    fb.items.add(
        juce::FlexItem(mRoomSizeMinSlider)
            .withMinWidth(itemWidth)
            .withMinHeight(itemHeight)
    );

    fb.items.add(
        juce::FlexItem(mRoomSizeMaxSlider)
            .withMinWidth(itemWidth)
            .withMinHeight(itemHeight)
    );

    fb.performLayout(bounds);
}
