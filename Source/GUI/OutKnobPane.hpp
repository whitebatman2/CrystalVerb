//
//  OutKnobPane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"


class OutKnobPane : public juce::GroupComponent
{
public:
	OutKnobPane(ReverbPluginAudioProcessor& audioProcessor);
	void resized() override;

private:
	using Label = juce::Label;

	AttachedToggle mBypassToggle;
	AttachedSlider mDryKnob;
	AttachedSlider mWetKnob;
    AttachedSlider mErKnob;

	Label mDryLabel;
	Label mWetLabel;
    Label mErLabel;
};
