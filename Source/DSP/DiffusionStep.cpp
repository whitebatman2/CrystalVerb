//
//  DiffusionStep.cpp
//  Miłosz Kolenderski
//

#include "DiffusionStep.hpp"
#include <cstdlib>
#include <cstring>


DiffusionStep::DiffusionStep()
    :
    mMatrixType(MatrixType::Hadamard)
{
}

DiffusionStep::~DiffusionStep()
{
}

void DiffusionStep::init(int numLines, MatrixType matrixType, double maxDelay)
{
    mDelayLines.init(numLines, maxDelay);
    setDelayLengths(maxDelay);
    mMatrixType = matrixType;

    for (int i = 0; i < numLines; ++i)
    {
        mFlipPolarity[i] = rand() % 2;
    }
}

void DiffusionStep::setDelayLengths(double maxDelay)
{
    int numLines = mDelayLines.getNumLines();
    int delayLengths[16] = {0};
    double delayStep = maxDelay / numLines;
    double delayMin = 0;

    for (int i = 0; i < numLines; ++i)
    {
        delayLengths[i] = rand() % int(ceil(delayStep)) + delayMin;
        delayMin += delayStep;
    }

    mDelayLines.setLineLengths(delayLengths);
}

void DiffusionStep::setMatrixType(MatrixType matrixType)
{
    mMatrixType = matrixType;
}
