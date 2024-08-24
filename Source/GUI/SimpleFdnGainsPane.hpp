//
//  SimpleFdnGainsPane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"
#include "../FdnConfig.hpp"
#include <array>


class SimpleFdnGainsPane : public juce::GroupComponent
{
public:
	SimpleFdnGainsPane(ReverbPluginAudioProcessor& audioProcessor);
	void resized() override;

private:
	std::array<std::unique_ptr<AttachedSlider>, FdnConfig::kFdnOrder> mGainSliders;
};
