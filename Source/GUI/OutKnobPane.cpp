//
//  OutKnobPane.cpp
//  Miłosz Kolenderski
//

#include "OutKnobPane.hpp"


using namespace Parameters;

OutKnobPane::OutKnobPane(ReverbPluginAudioProcessor& audioProcessor)
	:
mBypassToggle(audioProcessor, PID::Bypass),
mDryKnob(audioProcessor, PID::Dry),
mWetKnob(audioProcessor, PID::Wet),
mErKnob(audioProcessor, PID::ErAmount)
{
	setText("Output");
	setTextLabelPosition(juce::Justification::centredTop);

	mBypassToggle.setButtonText(getNameString(PID::Bypass));
	addAndMakeVisible(mBypassToggle);

	mDryKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mDryKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 48, 24);
	addAndMakeVisible(mDryKnob);
    mWetKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mWetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 48, 24);
	addAndMakeVisible(mWetKnob);
    mErKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mErKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 48, 24);
    addAndMakeVisible(mErKnob);

	mDryLabel.setText("Dry", juce::dontSendNotification);
	mDryLabel.attachToComponent(&mDryKnob, false);
	mWetLabel.setText("Wet", juce::dontSendNotification);
	mWetLabel.attachToComponent(&mWetKnob, false);
    mErLabel.setText("Early reflections", juce::dontSendNotification);
    mErLabel.attachToComponent(&mErKnob, false);

}

void OutKnobPane::resized()
{
	using Margin = juce::FlexItem::Margin;

	auto bounds = getLocalBounds();
	auto margin = 12;

	bounds.removeFromTop(24);
	bounds.removeFromLeft(margin);
	bounds.removeFromRight(margin);

	auto itemHeight = 24;
	auto knobSize = 90;
	auto itemWidth = 100;

	juce::FlexBox fb;
	fb.flexDirection = juce::FlexBox::Direction::column;
	fb.flexWrap = juce::FlexBox::Wrap::wrap;
	fb.alignContent = juce::FlexBox::AlignContent::stretch;
	fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;

	fb.items.add(
		juce::FlexItem(mDryKnob)
		.withMinWidth(knobSize)
		.withMinHeight(knobSize)
	);

	fb.items.add(
		juce::FlexItem(mWetKnob)
		.withMinWidth(knobSize)
		.withMinHeight(knobSize)
	);

    fb.items.add(
        juce::FlexItem(mErKnob)
        .withMinWidth(knobSize)
        .withMinHeight(knobSize)
    );

	fb.items.add(
		juce::FlexItem(mBypassToggle)
		.withMinWidth(itemWidth)
		.withMinHeight(itemHeight)
	);

	fb.performLayout(bounds);
}
