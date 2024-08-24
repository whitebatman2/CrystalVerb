//
//  FilterBank.cpp
//  Miłosz Kolenderski
//

#include "FilterBank.hpp"


FilterBank::FilterBank(int numSplits, const double* bandSplits, const double* gains,
                       double sampleRate, int filterOrder)
{
    using FilterDesign = juce::dsp::FilterDesign<double>;

    mNumBands = numSplits + 1;
    mSampleRate = sampleRate;
    mFilterOrder = filterOrder;

    for (int i = 0; i < numSplits; ++i)
    {
        auto coeffsLP = FilterDesign::designIIRLowpassHighOrderButterworthMethod(bandSplits[i], sampleRate, filterOrder);
        auto coeffsHP = FilterDesign::designIIRHighpassHighOrderButterworthMethod(bandSplits[i], sampleRate, filterOrder);

        if (i == 0)
        {
            mLOPerHO = coeffsLP.size();
        }

        for (int j = 0; j < mLOPerHO; ++j)
        {
            mBandSplitFilters.emplace_back(std::make_unique<IIRFilter>(coeffsHP[j]));
        }

        for (int j = 0; j < mLOPerHO; ++j)
        {
            mBandSplitFilters.emplace_back(std::make_unique<IIRFilter>(coeffsLP[j]));
        }

        for (int j = 0; j < i; ++j)
        {
            auto coeffsAPL = FilterDesign::designIIRLowpassHighOrderButterworthMethod(bandSplits[i], sampleRate, filterOrder);
            auto coeffsAPH = FilterDesign::designIIRHighpassHighOrderButterworthMethod(bandSplits[i], sampleRate, filterOrder);

            for (int k = 0; k < mLOPerHO; ++k)
            {
                mAllPassEqFilters.emplace_back(std::make_unique<IIRFilter>(coeffsAPH[k]));
            }

            for (int k = 0; k < mLOPerHO; ++k)
            {
                mAllPassEqFilters.emplace_back(std::make_unique<IIRFilter>(coeffsAPL[k]));
            }
        }
    }

    mDataLines = new double[mNumBands];
    mGains = new double[mNumBands];

    memcpy(mGains, gains, mNumBands * sizeof(double));
}

FilterBank::~FilterBank()
{
    delete[] mDataLines;
    delete[] mGains;
}

void FilterBank::setGains(const double* gains)
{
    memcpy(mGains, gains, mNumBands * sizeof(double));
}

void FilterBank::setGain(int band, double gain)
{
    if (band < mNumBands)
    {
        mGains[band] = gain;
    }
}

void FilterBank::changeSplitFrequency(int splitIdx, double frequency)
{
    using FilterDesign = juce::dsp::FilterDesign<double>;

    if (splitIdx >= mNumBands - 1)
    {
        return;
    }

    auto coeffsLP = FilterDesign::designIIRLowpassHighOrderButterworthMethod(frequency, mSampleRate, mFilterOrder);
    auto coeffsHP = FilterDesign::designIIRHighpassHighOrderButterworthMethod(frequency, mSampleRate, mFilterOrder);

    auto filterIdx = 2 * mLOPerHO * splitIdx;

    for (int i = 0; i < mLOPerHO; ++i)
    {
        mBandSplitFilters[filterIdx++]->coefficients = coeffsHP[i];
    }

    for (int i = 0; i < mLOPerHO; ++i)
    {
        mBandSplitFilters[filterIdx++]->coefficients = coeffsLP[i];
    }

    auto allPassIdx = (splitIdx - 1) * splitIdx * mLOPerHO;

    for (int j = 0; j < splitIdx; ++j)
    {
        auto coeffsAPL = FilterDesign::designIIRLowpassHighOrderButterworthMethod(frequency, mSampleRate, mFilterOrder);
        auto coeffsAPH = FilterDesign::designIIRHighpassHighOrderButterworthMethod(frequency, mSampleRate, mFilterOrder);

        for (int k = 0; k < mLOPerHO; ++k)
        {
            mAllPassEqFilters[allPassIdx++]->coefficients = coeffsAPH[k];
        }

        for (int k = 0; k < mLOPerHO; ++k)
        {
            mAllPassEqFilters[allPassIdx++]->coefficients = coeffsAPL[k];
        }
    }
}
