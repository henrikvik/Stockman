#include "Animation.h"

Animation::Animation(float totalDuration, std::initializer_list<KeyFrame> keyFrames)
    : totalDuration(totalDuration)
    , keyFrames(keyFrames)
{
}

std::pair<KeyFrame const&, KeyFrame const&> Animation::getKeyFrames(float duration)
{
    
    int prevKeyFrame = 0;
    int nextKeyFrame = 0;

    for (int i = 0; i < keyFrames.size(); i++)
    {
        if (keyFrames[i].getTimeStamp() < duration)
        {
            prevKeyFrame = i;
            nextKeyFrame = (i + 1) % keyFrames.size();
        }
    }

    return { keyFrames[prevKeyFrame], keyFrames[nextKeyFrame] };
}
