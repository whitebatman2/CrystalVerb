//
//  MultiDelayLine.cpp
//  Miłosz Kolenderski
//

#include "MultiDelayLine.hpp"


MultiDelayLine::MultiDelayLine()
{
}

MultiDelayLine::~MultiDelayLine()
{
    delete[] mDelayLines;
}

void MultiDelayLine::init(int numLines, int maxDelayLength)
{
    mMaxDelayLength = maxDelayLength;
    setNumLines(numLines);
}

void MultiDelayLine::setNumLines(int numLines)
{
    if (numLines != mNumLines)
    {
        if (mDelayLines)
        {
            delete[] mDelayLines;
        }
        
        mNumLines = numLines;
        mDelayLines = new DelayLine[numLines];

        for (int i = 0; i < mNumLines; ++i)
        {
            mDelayLines[i].setMaxDelay(mMaxDelayLength);
        }
    }
}

int MultiDelayLine::getNumLines()
{
    return mNumLines;
}

void MultiDelayLine::setLineLengths(const int* delaySamples)
{
    for (int i = 0; i < mNumLines; ++i)
    {
        mDelayLines[i].setDelay(delaySamples[i]);
        mDelayLines[i].clear();
    }
}

float MultiDelayLine::getLength(int i)
{
    return mDelayLines[i].getLength();
}
