//
//  RoomSizePane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"


class RoomSizePane : public juce::GroupComponent {
public:
    RoomSizePane(ReverbPluginAudioProcessor& audioProcessor);
    void resized() override;

private:
    AttachedSlider mRoomSizeMinSlider;
    AttachedSlider mRoomSizeMaxSlider;
    juce::Label mRoomSizeMinLabel;
    juce::Label mRoomSizeMaxLabel;
};
