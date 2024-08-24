//
//  FilterBank.hpp
//  Miłosz Kolenderski
//

#ifndef FilterBank_hpp
#define FilterBank_hpp

#include <stdio.h>
#include <vector>
#include <juce_dsp/juce_dsp.h>

//template <typename SampleType>
class FilterBank
{
using IIRFilter = juce::dsp::IIR::Filter<double>;

public:
    FilterBank(int numSplits, const double* bandSplits, const double* gains,
               double sampleRate, int filterOrder);
    ~FilterBank();

    inline double processSample(double sample);
    void setGains(const double* gains);
    void setGain(int band, double gain);
    void changeSplitFrequency(int splitIdx, double frequency);

private:
    int mNumBands;
    int mFilterOrder;
    int mLOPerHO; // How many low order filters per single filter
    double* mGains;
    double* mDataLines;
    double mSampleRate;

    std::vector<std::unique_ptr<IIRFilter>> mBandSplitFilters;
    std::vector<std::unique_ptr<IIRFilter>> mAllPassEqFilters;
};

double FilterBank::processSample(double sample)
{
    mDataLines[0] = sample;

    int bandPassFilterIndex = 0;
    int allPassFilterIndex = 0;
    double hp, lp;

    for (int i = 0; i < mNumBands - 1; ++i)
    {
        mDataLines[i + 1] = mDataLines[i];

        for (int j = 0; j < mLOPerHO; ++j)
        {
            // High-pass
            mDataLines[i] = mBandSplitFilters[bandPassFilterIndex++]->processSample(mDataLines[i]);
        }

        for (int j = 0; j < mLOPerHO; ++j)
        {
            // Low-pass
            mDataLines[i + 1] = mBandSplitFilters[bandPassFilterIndex++]->processSample(mDataLines[i + 1]);
        }

        // All-pass
        for (int j = 0; j <= i - 2; ++j)
        {
            hp = lp = mDataLines[j];

            // High-pass
            for (int k = 0; k < mLOPerHO; ++k)
            {
                hp = mAllPassEqFilters[allPassFilterIndex++]->processSample(hp);
            }

            // Low-pass
            for (int k = 0; k < mLOPerHO; ++k)
            {
                lp = mAllPassEqFilters[allPassFilterIndex++]->processSample(lp);
            }

            mDataLines[j] = hp + lp;
        }
    }

    double out = 0;
    for (int i = 0; i < mNumBands; ++i)
    {
        out += mGains[i] * mDataLines[i];
    }

    return out;
}

#endif /* FilterBank_hpp */
