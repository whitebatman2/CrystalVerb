//
//  MultiDelayLine.hpp
//  Miłosz Kolenderski
//

#ifndef MultiDelayLine_hpp
#define MultiDelayLine_hpp

#include <stdio.h>
#include "DelayLine.hpp"


class MultiDelayLine
{
public:
    MultiDelayLine();
    ~MultiDelayLine();

    void init(int numLines, int maxDelayLength);
    void setNumLines(int numBuffers);
    int getNumLines();
    void setLineLengths(const int* delaySamples);
    float getLength(int i);
    inline void writeSample(const double* inputs);
    inline void readSample(double* outputs);

private:
    DelayLine* mDelayLines = NULL;
    int mNumLines = 0;
    int mMaxDelayLength = 0;
};

void MultiDelayLine::writeSample(const double* inputs)
{
    for (int i = 0; i < mNumLines; ++i)
    {
        mDelayLines[i].pushSample(inputs[i]);
    }
}

void MultiDelayLine::readSample(double* outputs)
{
    for (int i = 0; i < mNumLines; ++i)
    {
        outputs[i] = mDelayLines[i].popSample();
    }
}

#endif /* MultiDelayLine_hpp */
