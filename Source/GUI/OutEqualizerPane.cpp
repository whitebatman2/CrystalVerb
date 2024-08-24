//
//  OutEqualizerPane.cpp
//  Miłosz Kolenderski
//

#include "OutEqualizerPane.hpp"


using namespace Parameters;

OutEqualizerPane::OutEqualizerPane(ReverbPluginAudioProcessor& audioProcessor)
	:
mOutEqToggle(audioProcessor, PID::OutEqEnable),
mOutEqualizer(audioProcessor, PID::Band1_GainOut, FdnConfig::kNumOutputFilterBands)
{
    setText("Output equalizer");
    setTextLabelPosition(juce::Justification::centredTop);

	mOutEqToggle.setButtonText("Enable");
	addAndMakeVisible(mOutEqToggle);

	mOutEqSplitLabel.setText("Out equalizer split frequencies", juce::NotificationType::dontSendNotification);
	addAndMakeVisible(mOutEqSplitLabel);

	mOutEqGainLabel.setText("Output equalizer", juce::NotificationType::dontSendNotification);
	mOutEqGainLabel.attachToComponent(&mOutEqualizer, false);

	addAndMakeVisible(mOutEqualizer);

	for (int i = 0; i < FdnConfig::kNumOutputFilterSplits; ++i)
	{
		auto pid = static_cast<int>(PID::CrossOver1Out) + i;
        mOutEqSplitFreqSliders[i] = std::make_unique<AttachedSlider>(audioProcessor, static_cast<PID>(pid));
		addAndMakeVisible(*mOutEqSplitFreqSliders[i]);
	}
}

void OutEqualizerPane::resized()
{
	using Margin = juce::FlexItem::Margin;

	auto bounds = getLocalBounds();
	auto margin = 12;

	bounds.removeFromTop(24);
	bounds.removeFromBottom(margin);
	bounds.removeFromLeft(margin);
	bounds.removeFromRight(margin);

	auto itemHeight = 24;
	auto itemWidth = 100;

	juce::FlexBox fb;
	fb.flexDirection = juce::FlexBox::Direction::column;
	fb.flexWrap = juce::FlexBox::Wrap::wrap;
	fb.alignContent = juce::FlexBox::AlignContent::stretch;
	fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

	fb.items.add(
		juce::FlexItem(mOutEqToggle)
		.withMinWidth(itemWidth)
		.withMinHeight(itemHeight)
		.withMargin(Margin(0, 0, margin / 2, 0))
	);

	fb.items.add(
		juce::FlexItem(mOutEqSplitLabel).withMinWidth(itemWidth).withMinHeight(itemHeight)
	);

	for (int i = 0; i < FdnConfig::kNumOutputFilterSplits; ++i)
	{
		fb.items.add(
			juce::FlexItem(*mOutEqSplitFreqSliders[i]).withMinWidth(itemWidth).withMinHeight(itemHeight)
		);
	}

	fb.items.add(
		juce::FlexItem(mOutEqualizer)
		.withMinWidth(itemWidth)
		.withMinHeight(100)
		.withFlex(5)
		.withMargin(Margin(itemHeight, 0, 0, 0))
	);

	fb.performLayout(bounds);
}
