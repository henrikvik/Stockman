#include "Animation.h"

Animation::Animation(float totalDuration, std::initializer_list<KeyFrame> keyFrames)
    : totalDuration(totalDuration)
    , keyFrames(keyFrames)
{
}

std::pair<KeyFrame const&, KeyFrame const&> Animation::getKeyFrames(float duration)
{
    int i = 0;
    for (; i < keyFrames.size() - 1; i++)
    {
        if (keyFrames[i].getTimeStamp() < duration)
        {
            break;
        }
    }
    return { keyFrames[i], keyFrames[i+1] };
}
