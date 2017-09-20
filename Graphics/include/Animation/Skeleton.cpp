#pragma once
#include "Skeleton.h"
#include "Animation.h"

Skeleton::Skeleton(Joint & rootJoint, int jointCount)
    : rootJoint(rootJoint)
    , jointCount(jointCount)
{
}

std::vector<Matrix> Skeleton::getJointTransforms(Animation & animation, float progress)
{
    auto keyFrames = animation.getKeyFrames(progress);

    std::vector<Matrix> animationTransforms = keyFrames.first.interpolateTo(keyFrames.second, progress);
    std::vector<Matrix> jointTransforms;



    return std::vector<Matrix>();
}