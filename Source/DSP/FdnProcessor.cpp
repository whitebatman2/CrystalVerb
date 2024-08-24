//
//  FdnProcessor.cpp
//  Miłosz Kolenderski
//

#include "FdnProcessor.hpp"
#include <JuceHeader.h>


FdnProcessor::FdnProcessor()
{
}

FdnProcessor::~FdnProcessor()
{
    delete[] mInputGain;
    delete[] mFeedbackGain;
    delete[] mLines;
}

void FdnProcessor::init(MatrixType feedback, int order, int sampleRate,
                        int numFreqBands, int filterOrder, const double* filterCross,
                        const double* bandGains, const double* staticGains)
{
    mMatrixOrder = order;
    mSampleRate = sampleRate;
    mMultiDelayLine.init(order, 8192 * sampleRate / 44100);

    mInputGain = new double[order];
    mFeedbackGain = new double[order];

    for (int i = 0; i < order; ++i)
    {
        mInputGain[i] = 1.0;
        mFeedbackGain[i] = staticGains[i];
    }

    mFeedbackType = feedback;

    mNumFreqBands = numFreqBands;

    for (int i = 0; i < order; ++i)
    {
        mFilterBanks.emplace_back(std::make_unique<FilterBank>(
            numFreqBands - 1, filterCross, bandGains,
            sampleRate, filterOrder));
    }

    mLines = new double[mMatrixOrder];
}

void FdnProcessor::calcDelayLengths(int* delayLengths, int order, int sampleRate,
                                    double roomSizeMin, double roomSizeMax)
{
    double delayMin = roomSizeMin / 343.0 * sampleRate;
    double delayMax = roomSizeMax / 343.0 * sampleRate;
    double step = (delayMax - delayMin) / order;

    for (int i = 0; i < order; ++i)
    {
        // Speed of sound: 343 m/s
        delayLengths[i] = (rand() % (int)step) + i * step + delayMin;
    }
}

void FdnProcessor::setDelayLengths(const int* delayLengths)
{
    mMultiDelayLine.setLineLengths(delayLengths);
}

void FdnProcessor::setRT60(const double* bandRT60)
{
    memcpy(mBandRT60, bandRT60, mNumFreqBands * sizeof(double));
}

void FdnProcessor::applyRT60()
{
    double gains[maxFilterBands] = {0};

    for (int i = 0; i < mMatrixOrder; ++i)
    {
        for (int j = 0; j < mNumFreqBands; ++j)
        {
            double loopMs = mMultiDelayLine.getLength(j) / mSampleRate;
            double loopsPerRT60 = mBandRT60[j] / loopMs;
            double dbPerCycle = -60 / loopsPerRT60;
            gains[j] = std::pow(10, dbPerCycle * 0.05); // Why 0.05?1
        }

        mFilterBanks[i]->setGains(gains);
    }
}

void FdnProcessor::setBandGains(const double *bandGains)
{
    for (int i = 0; i < mMatrixOrder; ++i)
    {
        mFilterBanks[i]->setGains(bandGains);
    }
}

void FdnProcessor::setBandGain(int bandIdx, double gain)
{
    if (bandIdx >= mNumFreqBands)
    {
        return;
    }

    for (int i = 0; i < mMatrixOrder; ++i)
    {
        mFilterBanks[i]->setGain(bandIdx, gain);
    }
}

void FdnProcessor::setStaticGain(int lineIdx, double gain)
{
    if (lineIdx >= mMatrixOrder)
    {
        return;
    }

    mFeedbackGain[lineIdx] = gain;
}

void FdnProcessor::setSplitFrequency(int splitIdx, double frequency)
{
    if (splitIdx >= mNumFreqBands - 1)
    {
        return;
    }

    for (int i = 0; i < mMatrixOrder; ++i)
    {
        mFilterBanks[i]->changeSplitFrequency(splitIdx, frequency);
    }
}

void FdnProcessor::setFeedbackType(MatrixType feedback)
{
    mFeedbackType = feedback;
}

void FdnProcessor::setEqualizerEnabled(bool enabled)
{
    mEqualizerEnabled = enabled;
}

int FdnProcessor::getOrder()
{
    return mMatrixOrder;
}
