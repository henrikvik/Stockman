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

    inline int getJointCount() const { return jointCount; }
private:
    Joint rootJoint;
    int jointCount;
};