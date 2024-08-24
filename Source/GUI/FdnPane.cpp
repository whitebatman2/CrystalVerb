//
//  FdnPane.cpp
//  Miłosz Kolenderski
//

#include "FdnPane.hpp"


using namespace Parameters;

FdnPane::FdnPane(ReverbPluginAudioProcessor& audioProcessor)
	:
mFdnEqToggle(audioProcessor, PID::FdnEqEnable),
mFdnDiffusionTypeComboBox(audioProcessor, PID::FeedbackType),
mFdnEqualizer(audioProcessor, PID::Band1_Gain, FdnConfig::kNumFeedbackFilterBands)
{
	setText("Feedback Delay Network");
	setTextLabelPosition(juce::Justification::centredTop);

	mFdnDiffusionTypeComboBox.addItemList(
		juce::StringArray{ "Hadamard", "Householder" },
		1);
    mFdnDiffusionTypeComboBox.sendInitialUpdate();

	addAndMakeVisible(mFdnDiffusionTypeComboBox);

	mFdnDiffusionMatrixLabel.setText(
		getNameString(PID::FeedbackType),
		juce::dontSendNotification);
	mFdnDiffusionMatrixLabel.attachToComponent(&mFdnDiffusionTypeComboBox, false);

	mFdnEqToggle.setButtonText("Enable FDN Equalizer");
	addAndMakeVisible(mFdnEqToggle);
	
	mFdnEqGainLabel.setText("FDN Equalizer", juce::NotificationType::dontSendNotification);
	mFdnEqGainLabel.attachToComponent(&mFdnEqualizer, false);

	addAndMakeVisible(mFdnEqualizer);

	mFdnEqSplitLabel.setText("FDN Equalizer split frequencies", juce::NotificationType::dontSendNotification);
	addAndMakeVisible(mFdnEqSplitLabel);

	for (int i = 0; i < FdnConfig::kNumFeedbackFilterSplits; ++i)
	{
		auto pid = static_cast<int>(PID::CrossOver1) + i;
		mFdnEqSplitFreqSliders[i] = std::make_unique<AttachedSlider>(audioProcessor, static_cast<PID>(pid));
		addAndMakeVisible(*mFdnEqSplitFreqSliders[i]);
	}
}

void FdnPane::resized()
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
		juce::FlexItem(mFdnDiffusionTypeComboBox)
			.withMinWidth(itemWidth)
			.withMinHeight(itemHeight)
			.withMargin(Margin(itemHeight, 0, 0, 0))
	);

	fb.items.add(
		juce::FlexItem(mFdnEqToggle)
			.withMinWidth(itemWidth)
			.withMinHeight(itemHeight)
			.withMargin(Margin(0, 0, margin / 2, 0))
	);

	fb.items.add(
		juce::FlexItem(mFdnEqSplitLabel).withMinWidth(itemWidth).withMinHeight(itemHeight)
	);

	for (int i = 0; i < FdnConfig::kNumFeedbackFilterSplits; ++i)
	{
		fb.items.add(
			juce::FlexItem(*mFdnEqSplitFreqSliders[i]).withMinWidth(itemWidth).withMinHeight(itemHeight)
		);
	}

	fb.items.add(
		juce::FlexItem(mFdnEqualizer)
			.withMinWidth(itemWidth)
			.withMinHeight(100)
			.withFlex(5)
			.withMargin(Margin(itemHeight, 0, 0, 0))
	);

	fb.performLayout(bounds);
}
