//
//  PluginEditor.hpp
//  Miłosz Kolenderski
//

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.hpp"
#include "GUI/AttachedWidgets.hpp"
#include "GUI/DiffusionPane.hpp"
#include "GUI/SimpleFdnGainsPane.hpp"
#include "GUI/FdnPane.hpp"
#include "GUI/OutEqualizerPane.hpp"
#include "GUI/OutKnobPane.hpp"
#include "GUI/RoomSizePane.hpp"


//==============================================================================
/**
*/
class ReverbPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReverbPluginAudioProcessorEditor (ReverbPluginAudioProcessor&);
    ~ReverbPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReverbPluginAudioProcessor& audioProcessor;

    std::array<AttachedToggle, 2> mToggles;
    AttachedComboBox mFeedbackType;
    AttachedSlider mRoomSizeMinSlider;
    AttachedSlider mRoomSizeMaxSlider;
    std::array<std::unique_ptr<AttachedSlider>, FdnConfig::kNumSliders> mSliders;

    juce::Label testLabel;
    DiffusionPane mDiffusionPane;
    SimpleFdnGainsPane mStaticFdnGainsPane;
    FdnPane mFdnPane;
    OutEqualizerPane mOutEqualizerPane;
    OutKnobPane mOutKnobPane;
    RoomSizePane mRoomSizePane;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbPluginAudioProcessorEditor)
};
