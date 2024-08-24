//
//  SimpleFdnGainsPane.cpp
//  Miłosz Kolenderski
//

#include "SimpleFdnGainsPane.hpp"


using namespace Parameters;

SimpleFdnGainsPane::SimpleFdnGainsPane(ReverbPluginAudioProcessor& audioProcessor)
{
	setText("Simple feedback gains");
	setTextLabelPosition(juce::Justification::centredTop);

	for (int i = 0; i < FdnConfig::kFdnOrder; ++i)
	{
		auto pid = static_cast<int>(PID::StaticGain1) + i;
		mGainSliders[i] = std::make_unique<AttachedSlider>(audioProcessor, static_cast<PID>(pid));
		mGainSliders[i]->setSliderStyle(juce::Slider::LinearVertical);
		mGainSliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 24);
		addAndMakeVisible(*mGainSliders[i]);
	}
}

void SimpleFdnGainsPane::resized()
{
	auto bounds = getLocalBounds();
	auto margin = 12;

	bounds.removeFromTop(24);
	bounds.removeFromBottom(margin);
	bounds.removeFromLeft(margin);
	bounds.removeFromRight(margin);

	juce::FlexBox fb;
	fb.flexDirection = juce::FlexBox::Direction::row;
	fb.flexWrap = juce::FlexBox::Wrap::wrap;
	fb.alignContent = juce::FlexBox::AlignContent::stretch;
	fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

	for (int i = 0; i < FdnConfig::kFdnOrder; ++i)
	{
		fb.items.add(
			juce::FlexItem(*mGainSliders[i]).withMinWidth(34).withMinHeight(100)
		);
	}

	fb.performLayout(bounds);
}
