//
//  DelayLine.cpp
//  Miłosz Kolenderski
//

#include "DelayLine.hpp"
#include <cmath>
#include <cstring>

DelayLine::DelayLine(int maxDelay)
{
    mAudioBuffer = new double[maxDelay];
    mBufferSize = maxDelay;
    mDelay = maxDelay;
    mBufferWritePos = 0;
}

DelayLine::~DelayLine()
{
    delete[] mAudioBuffer;
}

void DelayLine::setMaxDelay(int maxDelay)
{
    if (mBufferSize != maxDelay)
    {
        delete[] mAudioBuffer;
        mAudioBuffer = new double[maxDelay];
        mBufferSize = maxDelay;
    }
}

void DelayLine::setDelay(int delay)
{
    if (delay > mBufferSize)
    {
        delay = mBufferSize;
    }
    mDelay = delay;
}

void DelayLine::clear()
{
    if (mAudioBuffer && mBufferSize != 0)
    {
        memset(mAudioBuffer, 0, mBufferSize * sizeof(double));
    }
}

int DelayLine::getLength()
{
    return mDelay;
}
