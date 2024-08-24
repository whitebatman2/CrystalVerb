//
//  Diffuser.hpp
//  Miłosz Kolenderski
//

#ifndef Diffuser_hpp
#define Diffuser_hpp

#include <stdio.h>
#include <memory>
#include <vector>
#include "DiffusionStep.hpp"
#include "MixingMatrix.hpp"


class Diffuser
{
using MatrixType = MixingMatrix::MatrixType;

public:
    Diffuser();
    ~Diffuser();
    void init(int order, MatrixType matrixType, int steps, int totalMaxDelay);
    void setDelayLengths(int totalMaxDelay);
    void setMatrixType(MatrixType matrixType);
    inline void processSample(double* data);

private:
    std::vector<std::unique_ptr<DiffusionStep>> mDiffusionSteps;
    int mOrder;
};


void Diffuser::processSample(double* data)
{
    for (int i = 0; i < mDiffusionSteps.size(); ++i)
    {
        mDiffusionSteps[i]->processSample(data);
    }
}

#endif /* Diffuser_hpp */
