//
//  PluginProcessor.h
//  Miłosz Kolenderski
//

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>
#include <array>
#include "DSP/FdnProcessor.hpp"
#include "DSP/FilterBank.hpp"
#include "DSP/Diffuser.hpp"
#include "Parameters.hpp"

//==============================================================================
/**
*/
class ReverbPluginAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
                             , public juce::AudioProcessorParameter::Listener
{
public:
    //==============================================================================
    ReverbPluginAudioProcessor();
    ~ReverbPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    template <typename SampleType>
    void actualProcessBlock(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages);

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    virtual void parameterValueChanged(int parameterIndex, float newValue) override;
    virtual void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

    std::array<juce::RangedAudioParameter*, Parameters::numberOfParameters> mParameters;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbPluginAudioProcessor)

    juce::AudioProcessorValueTreeState mValueTreeState;
    juce::PerformanceCounter mPerfCounter;
    FdnProcessor mFdnProcessor;
    Diffuser mDiffuser;
    std::array<std::unique_ptr<FilterBank>, 2> mOutFilterBanks;

    int mSampleRate;
    int mDelayLengths[16];

    double mInputLines[16];
    double mOutputLines[16];
    double mEarlyReflections[16];
    double mDry;
    double mWet;
    double mErAmount;
    bool mDiffusionEnabled;
    bool mOutEqualizerEnabled;
    bool mInitialized;
};
