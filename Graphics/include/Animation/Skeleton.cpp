#pragma once
#include "Skeleton.h"
#include "Animation.h"
#include <minmax.h>

Skeleton::Skeleton(Joint & rootJoint, int jointCount)
    : rootJoint(rootJoint)
    , jointCount(jointCount)
{
}

Skeleton::~Skeleton()
{
}

std::vector<Matrix> Skeleton::getJointTransforms(Animation & animation, float duration)
{   auto keyFrames = animation.getKeyFrames(duration);
    float first = keyFrames.first.getTimeStamp();
    float second = keyFrames.second.getTimeStamp();

    float progress = (duration - first) / (second - first + 0.001f);

    printf("%f, %f -> %f\n", duration, first, second);

    std::vector<Matrix> animationTransforms = keyFrames.first.interpolateTo(keyFrames.second, progress);
    std::vector<Matrix> jointTransforms(jointCount);

    rootJoint.getJointTransforms(animationTransforms, jointTransforms);

    return jointTransforms;
}