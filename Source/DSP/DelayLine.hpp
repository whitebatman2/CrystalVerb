//
//  DelayLine.hpp
//  Miłosz Kolenderski
//

#ifndef DelayLine_hpp
#define DelayLine_hpp

#include <cstdio>

class DelayLine
{
public:
    const static int mDefaultBufferSize = 8192;

    DelayLine(int maxDelay = mDefaultBufferSize);
    ~DelayLine();
    void setMaxDelay(int maxDelay);
    void setDelay(int delay);
    inline void pushSample(double sample);
    inline double popSample();
    void clear();
    int getLength();

private:
    int mDelay;
    double* mAudioBuffer = NULL;
    int mBufferSize;
    int mBufferWritePos;
};

void DelayLine::pushSample(double sample)
{
    mAudioBuffer[mBufferWritePos] = sample;
    ++mBufferWritePos;
    if (mBufferWritePos >= mBufferSize)
    {
        mBufferWritePos -= mBufferSize;
    }
}

double DelayLine::popSample()
{
    int index = mBufferWritePos - mDelay;
    if (index < 0)
    {
        index += mBufferSize;
    }

    return mAudioBuffer[index];
}

#endif /* DelayLine_hpp */
