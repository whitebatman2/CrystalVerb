//
//  OutEqualizerPane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"
#include "EqualizerPane.hpp"


class OutEqualizerPane : public juce::GroupComponent
{
public:
	OutEqualizerPane(ReverbPluginAudioProcessor& audioProcessor);
	void resized() override;

private:
	AttachedToggle mOutEqToggle;
	juce::Label mOutEqSplitLabel;
	juce::Label mOutEqGainLabel;
	std::array<std::unique_ptr<AttachedSlider>, FdnConfig::kNumOutputFilterSplits> mOutEqSplitFreqSliders;
	EqualizerPane mOutEqualizer;
};

