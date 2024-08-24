//
//  DiffusionPanes.cpp
//  Miłosz Kolenderski
//

#include "DiffusionPane.hpp"

using namespace Parameters;

DiffusionPane::DiffusionPane(ReverbPluginAudioProcessor& audioProcessor)
	:
	mDiffusionToggle(audioProcessor, PID::DiffusionEnable),
	mDiffusionMatrixComboBox(audioProcessor, PID::DiffusionType)
{
	setText("Diffusion");
	setTextLabelPosition(juce::Justification::centredTop);

	mDiffusionToggle.setButtonText("Enabled");

	mDiffusionMatrixComboBox.addItemList(
		juce::StringArray{ "Hadamard", "Householder" },
		1);
    mDiffusionMatrixComboBox.sendInitialUpdate();

	mDiffusionMatrixLabel.setText(
		getNameString(PID::DiffusionType),
		juce::dontSendNotification);
	mDiffusionMatrixLabel.attachToComponent(&mDiffusionMatrixComboBox, false);

	addAndMakeVisible(mDiffusionToggle);
	addAndMakeVisible(mDiffusionMatrixComboBox);
}

void DiffusionPane::resized()
{
	auto bounds = getLocalBounds();
	auto margin = 12.f;
	auto itemHeight = 24;
	auto itemWidth = 200;

	bounds.removeFromTop(margin);
//	bounds.removeFromBottom(margin);
	bounds.removeFromLeft(margin);
	bounds.removeFromRight(margin);

	juce::FlexBox fb;
	fb.flexDirection = juce::FlexBox::Direction::column;
	fb.flexWrap = juce::FlexBox::Wrap::wrap;
	fb.alignContent = juce::FlexBox::AlignContent::stretch;
	fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	
	auto itemMargin = juce::FlexItem::Margin{ 0, 0, margin, 0 };

	fb.items.add(
		juce::FlexItem(mDiffusionToggle)
			.withMinWidth(itemWidth)
			.withMinHeight(itemHeight)
			.withMargin(itemMargin)
	);

	fb.items.add(
		juce::FlexItem(mDiffusionMatrixComboBox)
			.withMinWidth(itemWidth)
			.withMinHeight(itemHeight)
			.withMargin(itemMargin)
	);

	fb.performLayout(bounds);
}
