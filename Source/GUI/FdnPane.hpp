//
//  FdnPane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"
#include "../FdnConfig.hpp"
#include "EqualizerPane.hpp"
#include <array>


class FdnPane : public juce::GroupComponent
{
public:
	FdnPane(ReverbPluginAudioProcessor& audioProcessor);
	void resized() override;

private:
	AttachedToggle mFdnEqToggle;
	AttachedComboBox mFdnDiffusionTypeComboBox;
	juce::Label mFdnDiffusionMatrixLabel;
	juce::Label mFdnEqSplitLabel;
	juce::Label mFdnEqGainLabel;
	std::array<std::unique_ptr<AttachedSlider>, FdnConfig::kNumFeedbackFilterSplits> mFdnEqSplitFreqSliders;
	EqualizerPane mFdnEqualizer;
};
