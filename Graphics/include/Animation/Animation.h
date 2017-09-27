#pragma once
#include <vector>
#include "KeyFrame.h"
#include <utility>

class Animation
{
public:
    Animation(float totalDuration, std::initializer_list<KeyFrame> keyFrames);

    std::pair<KeyFrame const&, KeyFrame const&> getKeyFrames(float duration);

    inline float getTotalDuration() const { return totalDuration; }
private:
    std::vector<KeyFrame> keyFrames;
    float totalDuration;
};