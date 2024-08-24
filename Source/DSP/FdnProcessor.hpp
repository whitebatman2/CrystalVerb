//
//  FdnProcessor.hpp
//  Miłosz Kolenderski
//

#ifndef FdnProcessor_hpp
#define FdnProcessor_hpp

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>
#include "MultiDelayLine.hpp"
#include "FilterBank.hpp"
#include "MixingMatrix.hpp"

class FdnProcessor
{
using MatrixType = MixingMatrix::MatrixType;

public:
    static constexpr double defRoomSizeMin = 20.0;
    static constexpr double defRoomSizeMax = 60.0;
    static constexpr int maxFilterBands = 8;
    
    FdnProcessor();
    ~FdnProcessor();

    void init(MatrixType feedback, int order, int sampleRate,
              int numFreqBands, int filterOrder, const double* filterCross,
              const double* bandGains, const double* staticGains);
    inline void processSample(const double* input, double* output);
    static void calcDelayLengths(int* delayLengths, int order, int sampleRate,
                                 double roomSizeMin, double roomSizeMax);
    void setDelayLengths(const int* delayLengths);
    void setRT60(const double* bandRT60);
    void applyRT60();
    void setBandGains(const double* bandGains);
    void setBandGain(int bandIdx, double gain);
    void setStaticGain(int lineIdx, double gain);
    void setSplitFrequency(int splitIdx, double frequency);
    void setFeedbackType(MatrixType feedback);
    void setEqualizerEnabled(bool enabled);
    int getOrder();

private:

    int mMatrixOrder = 0;
    int mSampleRate = 0;
    MultiDelayLine mMultiDelayLine;
    MatrixType mFeedbackType;
    std::vector<std::unique_ptr<FilterBank>> mFilterBanks;
    double *mFeedbackGain = nullptr;
    double *mInputGain = nullptr;
    int mNumFreqBands;
    double mBandRT60[maxFilterBands];
    double* mLines = nullptr;
    bool mEqualizerEnabled = true;
};

void FdnProcessor::processSample(const double* input, double* output)
{
    using namespace MixingMatrix;

    mMultiDelayLine.readSample(mLines);

    for (int i = 0; i < mMatrixOrder; ++i)
    {
        output[i] = mLines[i];
    }

    if (mFeedbackType == MatrixType::Hadamard)
    {
        hadamardInPlace(mLines, mMatrixOrder);
    }
    else if (mFeedbackType == MatrixType::Householder)
    {
        householderInPlace(mLines, mMatrixOrder);
    }

    for (int i = 0; i < mMatrixOrder; ++i)
    {
        if (mEqualizerEnabled)
        {
            mLines[i] = mLines[i] + input[i];
            mLines[i] = mFilterBanks[i]->processSample(mLines[i]);
        }
        else
        {
            mLines[i] = mLines[i] + input[i];
            mLines[i] *= mFeedbackGain[i];
        }
    }

    mMultiDelayLine.writeSample(mLines);
}


#endif /* FdnProcessor_hpp */
