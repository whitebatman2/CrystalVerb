//
//  DiffusionStep.hpp
//  Miłosz Kolenderski
//

#ifndef DiffusionStep_hpp
#define DiffusionStep_hpp

#include "MultiDelayLine.hpp"
#include "MixingMatrix.hpp"
#include <stdio.h>


class DiffusionStep
{
using MatrixType = MixingMatrix::MatrixType;

public:
    DiffusionStep();
    ~DiffusionStep();
    void init(int numLines, MatrixType matrixType, double maxDelay);
    void setDelayLengths(double maxDelay);
    void setMatrixType(MatrixType matrixType);
    inline void processSample(double* data);

private:
    MultiDelayLine mDelayLines;
    MatrixType mMatrixType;
    bool mFlipPolarity[16];
};


void DiffusionStep::processSample(double* data)
{
    using namespace MixingMatrix;

    double output[16] = {0};

    mDelayLines.readSample(output);
    mDelayLines.writeSample(data);

    if (mMatrixType == MatrixType::Hadamard)
    {
        hadamardInPlace(output, mDelayLines.getNumLines());
    }
    else if (mMatrixType == MatrixType::Householder)
    {
        householderInPlace(output, mDelayLines.getNumLines());
    }

    for (int i = 0; i < mDelayLines.getNumLines(); ++i)
    {
        if (mFlipPolarity[i])
        {
            data[i] = -output[i];
        }
        else
        {
            data[i] = output[i];
        }
    }
}

#endif /* DiffusionStep_hpp */
