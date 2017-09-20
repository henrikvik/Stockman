#pragma once
#include <vector>
#include <initializer_list>
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
#include "Joint.h"
#include "JointTransform.h"

class KeyFrame
{
public:
    KeyFrame(float timeStamp, std::initializer_list<JointTransform>  jointTransforms);
    virtual ~KeyFrame();

    inline float getTimeStamp() const { return timeStamp; }

    std::vector<Matrix> interpolateTo(KeyFrame const& keyFrame, float progress) const;
private:
    float timeStamp;
    std::vector<JointTransform> jointTransforms;
};