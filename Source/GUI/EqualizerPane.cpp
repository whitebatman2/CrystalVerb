//
//  EqualizerPane.cpp
//  Miłosz Kolenderski
//

#include "EqualizerPane.hpp"


using namespace Parameters;

EqualizerPane::EqualizerPane(ReverbPluginAudioProcessor& audioProcessor,
                             Parameters::PID band1Gain,
                             unsigned int numBands)
{
	for (int i = 0; i < numBands; ++i)
	{
		auto pid = static_cast<int>(band1Gain) - i + numBands - 1;
		mGainSliders.push_back(
            std::make_unique<AttachedSlider>(audioProcessor, static_cast<PID>(pid))
        );
		mGainSliders[i]->setSliderStyle(juce::Slider::LinearVertical);
		mGainSliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 24);
		addAndMakeVisible(*mGainSliders[i]);
	}
}

void EqualizerPane::resized()
{
	auto bounds = getLocalBounds();
	auto margin = 12;

	bounds.removeFromBottom(margin);
	bounds.removeFromLeft(margin);
	bounds.removeFromRight(margin);

	juce::FlexBox fb;
	fb.flexDirection = juce::FlexBox::Direction::row;
	fb.flexWrap = juce::FlexBox::Wrap::wrap;
	fb.alignContent = juce::FlexBox::AlignContent::stretch;
	fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

	for (int i = 0; i < mGainSliders.size(); ++i)
	{
		fb.items.add(
			juce::FlexItem(*mGainSliders[i]).withMinWidth(50).withMinHeight(50)
		);
	}

	fb.performLayout(bounds);
}
