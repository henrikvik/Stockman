#pragma once
#include <vector>
#include "Joint.h"

class Animation;

class Skeleton
{
public:
    Skeleton(Joint & rootJoint, int jointCount);
    virtual ~Skeleton();

    std::vector<Matrix> getJointTransforms(Animation & animation, float progress);
private:
    Joint rootJoint;
    int jointCount;
};