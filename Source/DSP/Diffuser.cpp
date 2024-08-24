//
//  Diffuser.cpp
//  Miłosz Kolenderski
//

#include "Diffuser.hpp"


Diffuser::Diffuser()
{
}

Diffuser::~Diffuser()
{
}

void Diffuser::init(int order, MatrixType feedbackType, int steps, int totalMaxDelay)
{
    mOrder = order;
    mDiffusionSteps.clear();

    double totalParts = (1 + steps) * steps / 2;
    double baseDelay = totalMaxDelay / totalParts;

    for (int i = 1; i <= steps; ++i)
    {
        mDiffusionSteps.push_back(std::make_unique<DiffusionStep>());
        mDiffusionSteps.back()->init(order, feedbackType, baseDelay * i);
    }
}

void Diffuser::setDelayLengths(int totalMaxDelay)
{
    size_t steps = mDiffusionSteps.size();

    double totalParts = (1 + steps) * steps / 2;
    double baseDelay = totalMaxDelay / totalParts;

    for (int i = 0; i < steps; ++i)
    {
        mDiffusionSteps[i]->setDelayLengths(baseDelay * (i + 1));
    }
}

void Diffuser::setMatrixType(MatrixType matrixType)
{
    for (int i = 0; i < mDiffusionSteps.size(); ++i)
    {
        mDiffusionSteps[i]->setMatrixType(matrixType);
    }
}
