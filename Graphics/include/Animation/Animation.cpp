#include "Animation.h"
#include <minmax.h>

Animation::Animation(float totalDuration, std::initializer_list<KeyFrame> keyFrames)
    : totalDuration(totalDuration)
    , keyFrames(keyFrames)
{
}

std::pair<KeyFrame const&, KeyFrame const&> Animation::getKeyFrames(float duration)
{
    
    int prevKeyFrame = 0;
    int nextKeyFrame = 0;

    for (int i = 0, size = keyFrames.size(); i < size; i++)
    {
        if (keyFrames[i].getTimeStamp() < duration)
        {
            prevKeyFrame = i;
            nextKeyFrame = min(i + 1, size - 1);
        }
    }

    return { keyFrames[prevKeyFrame], keyFrames[nextKeyFrame] };
}
