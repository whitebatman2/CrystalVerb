//
//  EqualizerPane.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "AttachedWidgets.hpp"
#include "../FdnConfig.hpp"
#include <array>

class EqualizerPane : public juce::Component
{
public:
	EqualizerPane(ReverbPluginAudioProcessor& audioProcessor,
                  Parameters::PID band1Gain,
                  unsigned int numBands);
	void resized() override;

private:
	std::vector<std::unique_ptr<AttachedSlider>> mGainSliders;
};
