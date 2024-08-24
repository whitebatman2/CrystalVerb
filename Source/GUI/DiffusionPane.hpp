//
//  DiffusionPane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"


class DiffusionPane : public juce::GroupComponent
{
public:
	DiffusionPane(ReverbPluginAudioProcessor& audioProcessor);
	void resized() override;

private:
	using Label = juce::Label;

	AttachedToggle mDiffusionToggle;
	AttachedComboBox mDiffusionMatrixComboBox;
	Label mDiffusionMatrixLabel;
};
