#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class JointTransform
{
public:
    JointTransform(Vector3 position, Quaternion rotation);
    virtual ~JointTransform();

    Matrix interpolateTo(JointTransform const& jointTransform, float progress) const;
private:
    Vector3 position;
    Quaternion rotation;
};