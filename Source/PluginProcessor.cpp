//
//  PluginProcessor.cpp
//  Miłosz Kolenderski
//

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/MixingMatrix.hpp"
#include <string>
#include "FdnConfig.hpp"

//==============================================================================
ReverbPluginAudioProcessor::ReverbPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        mValueTreeState (*this, nullptr, "PARAMETERS", Parameters::createLayout()),
        mPerfCounter("audioProcessing", 100),
        mInitialized(false)
#endif
{
    for (int i = 0; i < Parameters::numberOfParameters; ++i)
    {
        mParameters[i] = mValueTreeState.getParameter(Parameters::getIDString(static_cast<Parameters::PID>(i)));
        mParameters[i]->addListener(this);
    }
}

ReverbPluginAudioProcessor::~ReverbPluginAudioProcessor()
{
}

//==============================================================================
const juce::String ReverbPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    using namespace Parameters;
    using namespace MixingMatrix;

    double filterCross[7];
    double bandGain[8];
    double staticGain[16];

    for (int i = static_cast<int>(PID::CrossOver1); i <= static_cast<int>(PID::CrossOver7); ++i)
    {
        const auto idx = i - static_cast<int>(PID::CrossOver1);
        filterCross[idx] = getParameterValueDenormalized(mParameters, static_cast<PID>(i));
    }

    for (int i = static_cast<int>(PID::Band1_Gain); i <= static_cast<int>(PID::Band8_Gain); ++i)
    {
        const auto idx = i - static_cast<int>(PID::Band1_Gain);
        bandGain[idx] = getParameterValueDenormalized(mParameters, static_cast<PID>(i));
    }

    for (int i = static_cast<int>(PID::StaticGain1); i <= static_cast<int>(PID::StaticGain16); ++i)
    {
        const auto idx = i - static_cast<int>(PID::StaticGain1);
        staticGain[idx] = getParameterValueDenormalized(mParameters, static_cast<PID>(i));
    }

    MatrixType feedbackType =
        static_cast<MatrixType>(
            getParameterValueDenormalized(mParameters, PID::FeedbackType));
    mFdnProcessor.init(feedbackType, FdnConfig::kFdnOrder, sampleRate,
                       FdnConfig::kNumFeedbackFilterBands, FdnConfig::kFeedbackFilterOrder,
                       filterCross, bandGain, staticGain);
    FdnProcessor::calcDelayLengths(mDelayLengths, mFdnProcessor.getOrder(), sampleRate,
                                   getParameterValueDenormalized(mParameters, PID::RoomSizeMin),
                                   getParameterValueDenormalized(mParameters, PID::RoomSizeMax));
    mFdnProcessor.setDelayLengths(mDelayLengths);


    mDry = getParameterValue(mParameters, PID::Dry);
    mWet = getParameterValue(mParameters, PID::Wet);

    MatrixType diffusionType =
        static_cast<MatrixType>(
            getParameterValueDenormalized(mParameters, PID::DiffusionType));
    mDiffuser.init(FdnConfig::kFdnOrder, diffusionType,
                   FdnConfig::kNumDiffuserStages, mDelayLengths[0]);

    mSampleRate = sampleRate;

    for (int i = static_cast<int>(PID::CrossOver1Out); i <= static_cast<int>(PID::CrossOver7Out); ++i)
    {
        const auto idx = i - static_cast<int>(PID::CrossOver1Out);
        filterCross[idx] = getParameterValueDenormalized(mParameters, static_cast<PID>(i));
    }

    for (int i = static_cast<int>(PID::Band1_GainOut); i <= static_cast<int>(PID::Band8_GainOut); ++i)
    {
        const auto idx = i - static_cast<int>(PID::Band1_GainOut);
        bandGain[idx] = getParameterValueDenormalized(mParameters, static_cast<PID>(i));
    }

    mOutFilterBanks[0] = std::make_unique<FilterBank>(
        FdnConfig::kNumOutputFilterSplits,
        filterCross,
        bandGain,
        sampleRate,
        FdnConfig::kOutputFilterOrder);
    mOutFilterBanks[1] = std::make_unique<FilterBank>(
        FdnConfig::kNumOutputFilterSplits,
        filterCross,
        bandGain,
        sampleRate,
        FdnConfig::kOutputFilterOrder);

    mDiffusionEnabled = getParameterValue(mParameters, PID::DiffusionEnable);
    mOutEqualizerEnabled = getParameterValue(mParameters, PID::OutEqEnable);
    mErAmount = getParameterValue(mParameters, PID::ErAmount);

    mInitialized = true;
}

void ReverbPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

template <typename SampleType>
void ReverbPluginAudioProcessor::actualProcessBlock(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages)
{
    using namespace Parameters;

    mPerfCounter.start();

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto totalNumSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    double fdnOrder = mFdnProcessor.getOrder();
    double dryL;
    double dryR;

    if (!getParameterValue(mParameters, PID::Bypass))
    {
        for (int i = 0; i < totalNumSamples; ++i)
        {
            for (int j = 0; j < fdnOrder; ++j)
            {
                mInputLines[j] = 0;
            }

            dryL = buffer.getSample(0, i);
            dryR = buffer.getSample(1, i);

            for (int j = 0; j < fdnOrder; j += 2)
            {
                mInputLines[j] = buffer.getSample(0, i) / fdnOrder;
                mInputLines[j + 1] = buffer.getSample(1, i) / fdnOrder;
            }

            if (mDiffusionEnabled)
            {
                mDiffuser.processSample(mInputLines);
                
                for (int j = 2; j < fdnOrder; ++j)
                {
                    mEarlyReflections[j] = mInputLines[j];
                }
            }
            mFdnProcessor.processSample(mInputLines, mOutputLines);

            double outL = 0;
            double outR = 0;
            for (int j = 0; j < fdnOrder; j += 2)
            {
                outL += mOutputLines[j] * 2;
                outR += mOutputLines[j + 1] * 2;
            }

            if (mOutEqualizerEnabled)
            {
                outL = mOutFilterBanks[0]->processSample(outL);
                outR = mOutFilterBanks[1]->processSample(outR);
            }

            outL = mWet * outL + mDry * dryL;
            outR = mWet * outR + mDry * dryR;

            if (mDiffusionEnabled)
            {
                for (int j = 0; j < fdnOrder; j += 2)
                {
                    outL += mEarlyReflections[j] * mErAmount / FdnConfig::kNumDiffuserStages * 2;
                    outR += mEarlyReflections[j + 1] * mErAmount / FdnConfig::kNumDiffuserStages * 2;
                }
            }

            buffer.setSample(0, i, outL);
            buffer.setSample(1, i, outR);
        }
    }

    mPerfCounter.stop();
}

void ReverbPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    actualProcessBlock(buffer, midiMessages);
}

void ReverbPluginAudioProcessor::processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    actualProcessBlock(buffer, midiMessages);
}

//==============================================================================
bool ReverbPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbPluginAudioProcessor::createEditor()
{
    return new ReverbPluginAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ReverbPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = mValueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ReverbPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (mValueTreeState.state.getType()))
        {
            mValueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbPluginAudioProcessor();
}

void ReverbPluginAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    using namespace Parameters;
    using namespace MixingMatrix;

    if (!mInitialized) {
        prepareToPlay(44100, 512);
    }

    if (parameterIndex == PID::Dry)
    {
        mDry = newValue;
    }
    else if (parameterIndex == PID::Wet)
    {
        mWet = newValue;
    }
    else if (parameterIndex == PID::FeedbackType)
    {
        mFdnProcessor.setFeedbackType(static_cast<MatrixType>(newValue));
    }
    else if (parameterIndex == PID::RoomSizeMin || parameterIndex == PID::RoomSizeMax)
    {
        double roomSizeMin = getParameterValueDenormalized(mParameters, PID::RoomSizeMin);
        double roomSizeMax = getParameterValueDenormalized(mParameters, PID::RoomSizeMax);

        FdnProcessor::calcDelayLengths(mDelayLengths, mFdnProcessor.getOrder(), mSampleRate,
                                       roomSizeMin, roomSizeMax);
        mFdnProcessor.setDelayLengths(mDelayLengths);
        mDiffuser.setDelayLengths(mDelayLengths[0]);
    }
    else if (parameterIndex >= static_cast<int>(PID::Band1_Gain)
             && parameterIndex <= static_cast<int>(PID::Band8_Gain))
    {
        int bandIdx = parameterIndex - static_cast<int>(PID::Band1_Gain);
        mFdnProcessor.setBandGain(bandIdx, newValue);
    }
    else if (parameterIndex >= static_cast<int>(PID::CrossOver1)
             && parameterIndex <= static_cast<int>(PID::CrossOver7))
    {
        int splitIdx = parameterIndex - static_cast<int>(PID::CrossOver1);
        mFdnProcessor.setSplitFrequency(splitIdx,
                                        getParameterValueDenormalized(mParameters, static_cast<PID>(parameterIndex)));
    }
    else if (parameterIndex >= static_cast<int>(PID::StaticGain1)
        && parameterIndex <= static_cast<int>(PID::StaticGain16))
    {
        int bandIdx = parameterIndex - static_cast<int>(PID::StaticGain1);
        mFdnProcessor.setStaticGain(bandIdx, newValue);
    }
    else if (parameterIndex == PID::DiffusionEnable)
    {
        mDiffusionEnabled = getParameterValue(mParameters, PID::DiffusionEnable);
    }
    else if (parameterIndex == PID::FdnEqEnable)
    {
        mFdnProcessor.setEqualizerEnabled(newValue);
    }
    else if (parameterIndex == PID::OutEqEnable)
    {
        mOutEqualizerEnabled = newValue;
    }
    else if (parameterIndex >= static_cast<int>(PID::Band1_GainOut)
          && parameterIndex <= static_cast<int>(PID::Band8_GainOut))
    {
        int bandIdx = parameterIndex - static_cast<int>(PID::Band1_GainOut);
        mOutFilterBanks[0]->setGain(bandIdx, newValue);
        mOutFilterBanks[1]->setGain(bandIdx, newValue);
    }
    else if (parameterIndex >= static_cast<int>(PID::CrossOver1Out)
          && parameterIndex <= static_cast<int>(PID::CrossOver7Out))
    {
        int splitIdx = parameterIndex - static_cast<int>(PID::CrossOver1Out);
        double newValue = getParameterValueDenormalized(mParameters, static_cast<PID>(parameterIndex));
        mOutFilterBanks[0]->changeSplitFrequency(splitIdx, newValue);
        mOutFilterBanks[1]->changeSplitFrequency(splitIdx, newValue);
    }
    else if (parameterIndex == PID::ErAmount)
    {
        mErAmount = newValue;
    }
    else if (parameterIndex == PID::DiffusionType)
    {
        mDiffuser.setMatrixType(static_cast<MatrixType>(newValue));
    }
}

void ReverbPluginAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}
