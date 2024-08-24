//
//  AttachedWidgets.hpp
//  Miłosz Kolenderski
//

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../Parameters.hpp"

template <typename Widget, typename Attachment>
class AttachedWidget : public Widget
{
public:
    AttachedWidget(ReverbPluginAudioProcessor& audioProcessor, Parameters::PID pid)
        :
        Widget(),
        mAttachment(*audioProcessor.mParameters[static_cast<int>(pid)], *this, nullptr)
    {
    }

    void sendInitialUpdate()
    {
        mAttachment.sendInitialUpdate();
    }

private:
    Attachment mAttachment;
};

using AttachedToggle = AttachedWidget<juce::ToggleButton, juce::ButtonParameterAttachment>;
using AttachedSlider = AttachedWidget<juce::Slider, juce::SliderParameterAttachment>;
using AttachedComboBox = AttachedWidget<juce::ComboBox, juce::ComboBoxParameterAttachment>;
